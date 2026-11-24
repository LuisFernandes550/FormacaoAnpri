"""Gera data/catalog.js a partir do repositório FormacaoAnpri e da pasta Drive."""

from __future__ import annotations

import io
import json
import re
import ssl
import unicodedata
import zipfile
from concurrent.futures import ThreadPoolExecutor, as_completed
from datetime import datetime, timezone
from html import unescape
from pathlib import Path
from urllib.error import HTTPError, URLError
from urllib.parse import quote
from urllib.request import Request, urlopen

ROOT = Path(__file__).resolve().parents[1]
SITE = ROOT / "docs"
OUT_JS = SITE / "data" / "catalog.js"
OUT_JSON = SITE / "data" / "catalog.json"

REPO = "LuisFernandes550/FormacaoAnpri"
BRANCH = "main"
DRIVE_FOLDER = "1S66_71QPppwhJrDpkIF-GApYYr0ZmdBV"
TREE_URL = f"https://api.github.com/repos/{REPO}/git/trees/HEAD?recursive=1"
RAW = f"https://raw.githubusercontent.com/{REPO}/{BRANCH}/"
BLOB = f"https://github.com/{REPO}/blob/{BRANCH}/"
TREE = f"https://github.com/{REPO}/tree/{BRANCH}/"
CDN = f"https://cdn.jsdelivr.net/gh/{REPO}@{BRANCH}/"

CODE_EXT = {".ino", ".pde", ".cpp", ".c", ".h", ".py"}
VIDEO_EXT = {".mp4", ".mov", ".webm", ".wmv", ".mpg", ".mpeg"}
SLIDE_EXT = {".ppt", ".pptx", ".pps", ".ppsx", ".odp"}
PDF_EXT = {".pdf"}
DOC_EXT = {".docx", ".doc", ".rtf"}
IMAGE_EXT = {".jpg", ".jpeg", ".png", ".webp", ".gif"}
SKIP_DIRS = {"lib", "libs", "library", "libraries", "node_modules", ".git"}

CTX = ssl.create_default_context()


def fetch(url: str, timeout: int = 40) -> bytes:
    req = Request(url, headers={"User-Agent": "FormacaoAnpriCatalog/1.0"})
    with urlopen(req, timeout=timeout, context=CTX) as res:
        return res.read()


def enc_path(path: str) -> str:
    return "/".join(quote(part) for part in path.split("/"))


def ext_of(path: str) -> str:
    dot = path.rfind(".")
    if dot < 0 or "/" in path[dot:]:
        return ""
    return path[dot:].lower()


def slugify(value: str) -> str:
    text = unicodedata.normalize("NFKD", value)
    text = text.encode("ascii", "ignore").decode("ascii")
    text = re.sub(r"[^a-zA-Z0-9]+", "-", text).strip("-").lower()
    return text or "projeto"


def strip_html(text: str) -> str:
    text = re.sub(r"(?is)<script.*?>.*?</script>", " ", text)
    text = re.sub(r"(?is)<style.*?>.*?</style>", " ", text)
    text = re.sub(r"(?i)<br\s*/?>", "\n", text)
    text = re.sub(r"(?i)</p>", "\n\n", text)
    text = re.sub(r"(?i)</h[1-6]>", "\n\n", text)
    text = re.sub(r"(?i)<a\s+[^>]*href=[\"']([^\"']+)[\"'][^>]*>.*?</a>", r" \1 ", text)
    text = re.sub(r"(?s)<[^>]+>", " ", text)
    return unescape(text)


def youtube_ids(text: str) -> list[str]:
    found = re.findall(
        r"(?:youtu\.be/|youtube\.com/(?:watch\?v=|embed/|shorts/)|img\.youtube\.com/vi/)([A-Za-z0-9_-]{11})",
        text,
    )
    seen: list[str] = []
    for item in found:
        if item not in seen:
            seen.append(item)
    return seen


def other_links(text: str) -> list[str]:
    urls = re.findall(r"https?://[^\s<>\"')\]]+", text)
    kept: list[str] = []
    for url in urls:
        url = url.rstrip(".,;")
        low = url.lower()
        if "youtu.be/" in low or "youtube.com/" in low or "img.youtube.com/" in low:
            continue
        if url not in kept:
            kept.append(url)
    return kept


def clean_line(line: str) -> str:
    line = line.strip()
    line = re.sub(r"^#+\s*", "", line)
    line = re.sub(r"[*_`]+", "", line)
    line = re.sub(r"^\s*[-*]\s+", "", line)
    line = re.sub(r"\s+", " ", line)
    return line.strip()


BOILER = re.compile(
    r"(projeto desenvolvido no âmbito|trabalho desenvolvido na ação|"
    r"programação com arduinos|associação de professores de informática|"
    r"créditos\s*:|link para vídeo|^vídeos?$|^video$)",
    re.IGNORECASE,
)


def parse_readme(raw: str, folder_name: str) -> dict:
    text = raw.replace("\r\n", "\n")
    title_match = re.search(r"(?m)^#\s+(.+)$", text)
    title = clean_line(strip_html(title_match.group(1))) if title_match else folder_name
    credits_match = re.search(
        r"Cr[eé]ditos:\s*(?:</b>)?\s*(?:\*\*)?\s*([^\n<]+)", text, re.IGNORECASE
    )
    author = clean_line(strip_html(credits_match.group(1))) if credits_match else ""
    videos = youtube_ids(text)
    links = other_links(text)

    plain = strip_html(text)
    paragraphs: list[str] = []
    for block in re.split(r"\n\s*\n", plain):
        lines = []
        for line in block.splitlines():
            cleaned = clean_line(line)
            if not cleaned or cleaned == title:
                continue
            if BOILER.search(cleaned):
                continue
            if re.fullmatch(r"https?://\S+", cleaned):
                continue
            if re.match(r"(?i)^v[ií]deo\s*\d+", cleaned) and "http" not in cleaned.lower():
                continue
            lines.append(cleaned)
        if lines:
            paragraphs.append(" ".join(lines))
    objectives = "\n\n".join(paragraphs).strip()
    if len(objectives) < 40:
        objectives = ""
    return {
        "title": title or folder_name,
        "author": author,
        "objectives": objectives,
        "youtube": videos,
        "links": links,
    }


def is_vendor(path: str) -> bool:
    parts = [part.lower() for part in path.split("/")]
    if any(part in SKIP_DIRS for part in parts):
        return True
    if any(part.endswith("-master") or "arduino-master" in part for part in parts):
        return True
    return False


def file_record(path: str, size: int) -> dict:
    name = path.rsplit("/", 1)[-1]
    encoded = enc_path(path)
    return {
        "name": name,
        "path": path,
        "size": size,
        "url": BLOB + encoded,
        "rawUrl": RAW + encoded,
        "cdnUrl": CDN + encoded,
    }


def fold(value: str) -> str:
    text = unicodedata.normalize("NFKD", value.strip().lower())
    text = "".join(char for char in text if not unicodedata.combining(char))
    return re.sub(r"\s+", " ", text)


PROPOSAL_HEADINGS = {
    "formando": "author",
    "designacao do projeto": "title",
    "designacao": "title",
    "enquadramento": "summary",
    "objetivos": "objectives",
    "objetivo": "objectives",
    "recursos necessarios": "resources",
    "recursos": "resources",
}


def docx_text(data: bytes) -> str:
    with zipfile.ZipFile(io.BytesIO(data)) as archive:
        xml = archive.read("word/document.xml").decode("utf-8", errors="replace")
    xml = xml.replace("</w:p>", "\n").replace("</w:tr>", "\n")
    xml = re.sub(r"<w:tab[^>]*/>", " ", xml)
    text = re.sub(r"<[^>]+>", "", xml)
    return unescape(text).replace("\xa0", " ")


def is_proposal(path: str) -> bool:
    name = path.rsplit("/", 1)[-1].lower()
    if not name.endswith(".docx"):
        return False
    if any(skip in name for skip in ("link", "esquema", "pcb", "componente", "compontes", "tinker", "thinker")):
        return False
    return any(
        key in name
        for key in ("proposta", "descri", "objetivo", "desenvolvimento", "projeto")
    )


def proposal_score(path: str) -> int:
    name = fold(path.rsplit("/", 1)[-1])
    if "proposta" in name:
        return 3
    if "descri" in name:
        return 2
    return 1


def parse_proposal(text: str) -> dict:
    sections: dict[str, list[str]] = {"_intro": []}
    current = "_intro"
    found_heading = False
    for raw_line in text.replace("\r\n", "\n").split("\n"):
        line = re.sub(r"\s+", " ", raw_line).strip()
        key = PROPOSAL_HEADINGS.get(fold(line))
        if key and len(line) < 40:
            current = key
            found_heading = True
            sections.setdefault(current, [])
            continue
        if line:
            sections.setdefault(current, []).append(line)

    def joined(name: str) -> str:
        return "\n".join(sections.get(name, [])).strip()

    parsed = {
        "title": joined("title"),
        "author": joined("author"),
        "summary": joined("summary"),
        "objectives": joined("objectives"),
        "resources": joined("resources"),
        "links": other_links(text),
    }
    if not found_heading:
        free = "\n".join(sections.get("_intro", [])).strip()
        if len(free) > 80:
            parsed["summary"] = free
    return parsed


def enrich_from_proposals(projects: list[dict], blobs: dict[str, dict]) -> None:
    chosen: dict[str, tuple[int, dict, str]] = {}
    for project in projects:
        folder = project["folder"] + "/"
        for path, node in blobs.items():
            if not path.startswith(folder) or not is_proposal(path):
                continue
            if int(node.get("size") or 0) > 1_500_000:
                continue
            score = proposal_score(path)
            current = chosen.get(project["id"])
            if current is None or score > current[0]:
                chosen[project["id"]] = (score, project, path)

    def load(path: str) -> str:
        try:
            return docx_text(fetch(RAW + enc_path(path), timeout=40))
        except (HTTPError, URLError, TimeoutError, zipfile.BadZipFile, KeyError):
            return ""

    with ThreadPoolExecutor(max_workers=8) as pool:
        futures = {
            pool.submit(load, path): (project, path)
            for _, project, path in chosen.values()
        }
        for future in as_completed(futures):
            project, path = futures[future]
            parsed = parse_proposal(future.result())
            if parsed["title"] and len(parsed["title"]) < 120:
                project["title"] = parsed["title"].split("\n")[0]
            if parsed["author"] and not project["author"]:
                project["author"] = parsed["author"].split("\n")[0]
            plain = re.sub(r"https?://\S+", " ", parsed["summary"])
            letters = len(re.findall(r"[A-Za-zÀ-ÿ]", plain))
            if letters >= 60:
                project["summary"] = parsed["summary"]
            if parsed["objectives"]:
                project["objectives"] = parsed["objectives"]
                project["objectivesSource"] = "proposta"
            if parsed["resources"]:
                project["resources"] = parsed["resources"]
            known = {item.get("url") for item in project["presentations"]}
            for link in parsed["links"]:
                if link not in known:
                    project["presentations"].append(
                        {"kind": "link", "name": link_label(link), "url": link}
                    )
                    known.add(link)


def edition_label(edition: str) -> str:
    match = re.fullmatch(r"(\d{4})_T(\d+)", edition)
    if not match:
        return edition
    return f"{match.group(1)} · Turma {match.group(2)}"


def load_tree() -> list[dict]:
    payload = json.loads(fetch(TREE_URL).decode("utf-8"))
    if payload.get("truncated"):
        raise SystemExit("A árvore do GitHub veio truncada.")
    return payload["tree"]


def project_folders(tree: list[dict]) -> list[str]:
    folders = []
    for node in tree:
        if node["type"] != "tree":
            continue
        parts = node["path"].split("/")
        if len(parts) == 2 and re.fullmatch(r"\d{4}_T\d+", parts[0]):
            folders.append(node["path"])
    return sorted(folders)


def fetch_readme(folder: str, blobs: dict[str, dict]) -> str:
    readme = next(
        (path for path in blobs if path.lower() == f"{folder}/readme.md".lower()),
        None,
    )
    if not readme:
        return ""
    try:
        return fetch(RAW + enc_path(readme), timeout=30).decode("utf-8", errors="replace")
    except (HTTPError, URLError, TimeoutError):
        return ""


def build_repo_projects(tree: list[dict]) -> list[dict]:
    blobs = {node["path"]: node for node in tree if node["type"] == "blob"}
    folders = project_folders(tree)
    readmes: dict[str, str] = {}
    with ThreadPoolExecutor(max_workers=12) as pool:
        futures = {pool.submit(fetch_readme, folder, blobs): folder for folder in folders}
        for future in as_completed(futures):
            readmes[futures[future]] = future.result()

    projects = []
    used_ids: set[str] = set()
    for folder in folders:
        edition, name = folder.split("/", 1)
        info = parse_readme(readmes.get(folder, ""), name)
        author = info["author"]
        if not author and " - " in name:
            author = name.split(" - ")[-1].strip()
        title = info["title"]
        if title.lower() in {"readme", "projeto", "projeto final"} or not title:
            title = name.split(" - ")[0].strip()
        files = [blobs[path] for path in blobs if path.startswith(folder + "/")]
        code = []
        presentations = []
        documents = []
        images = []
        for node in files:
            path = node["path"]
            size = int(node.get("size") or 0)
            extension = ext_of(path)
            record = file_record(path, size)
            if extension in CODE_EXT and not is_vendor(path):
                record["language"] = "python" if extension == ".py" else "cpp"
                record["sketch"] = extension in {".ino", ".pde"}
                code.append(record)
            elif extension in VIDEO_EXT:
                record["kind"] = "video"
                presentations.append(record)
            elif extension in SLIDE_EXT:
                record["kind"] = "slides"
                presentations.append(record)
            elif extension in PDF_EXT:
                record["kind"] = "pdf"
                presentations.append(record)
            elif extension in DOC_EXT:
                record["kind"] = "document"
                documents.append(record)
            elif extension in IMAGE_EXT and size <= 2_500_000 and not is_vendor(path):
                images.append(record)

        code.sort(key=lambda item: (not item["sketch"], item["path"].count("/"), item["path"].lower()))
        code = code[:30]
        images = images[:6]
        for video in info["youtube"]:
            presentations.insert(
                0,
                {
                    "kind": "youtube",
                    "name": "Vídeo",
                    "id": video,
                    "url": f"https://youtu.be/{video}",
                },
            )
        for link in info["links"]:
            presentations.append({"kind": "link", "name": link_label(link), "url": link})

        project_id = unique_id(f"{edition}--{slugify(name)}", used_ids)
        projects.append(
            {
                "id": project_id,
                "source": "github",
                "edition": edition,
                "editionLabel": edition_label(edition),
                "title": title,
                "author": author,
                "summary": "",
                "objectives": info["objectives"],
                "objectivesSource": "readme" if info["objectives"] else "",
                "resources": "",
                "folder": folder,
                "githubUrl": TREE + enc_path(folder),
                "presentations": presentations,
                "documents": documents[:12],
                "code": code,
                "images": images,
            }
        )
    enrich_from_proposals(projects, blobs)
    projects.sort(key=lambda item: (item["edition"], item["title"].lower()))
    return projects


def unique_id(base: str, used: set[str]) -> str:
    candidate = base
    index = 2
    while candidate in used:
        candidate = f"{base}-{index}"
        index += 1
    used.add(candidate)
    return candidate


def link_label(url: str) -> str:
    low = url.lower()
    if "genial.ly" in low:
        return "Apresentação Genially"
    if "canva.com" in low:
        return "Apresentação Canva"
    if "tinkercad.com" in low:
        return "Circuito no Tinkercad"
    if "drive.google.com" in low:
        return "Ficheiro no Google Drive"
    if "photos.google" in low or "photos.app.goo.gl" in low:
        return "Fotografias"
    return "Ligação externa"


def parse_drive_folder() -> list[dict]:
    html = fetch(
        f"https://drive.google.com/embeddedfolderview?id={DRIVE_FOLDER}"
    ).decode("utf-8", errors="replace")
    entries = []
    for match in re.finditer(
        r'id="entry-([^"]+)"[\s\S]*?flip-entry-title">([^<]+)', html
    ):
        entries.append((match.group(1), unescape(match.group(2)).strip()))
    return entries


def parse_links_doc(file_id: str) -> list[dict]:
    try:
        raw = fetch(
            f"https://docs.google.com/document/d/{file_id}/export?format=txt"
        ).decode("utf-8", errors="replace")
    except (HTTPError, URLError, TimeoutError):
        return []
    projects = []
    title = ""
    notes: list[str] = []
    urls: list[str] = []

    def flush() -> None:
        nonlocal title, notes, urls
        if title and (urls or notes):
            projects.append({"title": title, "notes": notes, "urls": urls})
        title, notes, urls = "", [], []

    for line in raw.replace("\r\n", "\n").split("\n"):
        stripped = line.strip()
        if not stripped:
            flush()
            continue
        if stripped.startswith("http://") or stripped.startswith("https://"):
            urls.append(stripped)
            continue
        if stripped.startswith("(") and title:
            notes.append(stripped.strip("() "))
            continue
        if title and urls:
            flush()
        if not title:
            title = stripped.rstrip(":")
        else:
            notes.append(stripped)
    flush()
    return projects


def presentation_from_url(url: str, note: str = "") -> dict:
    ids = youtube_ids(url)
    if ids:
        return {"kind": "youtube", "name": note or "Vídeo", "id": ids[0], "url": url}
    drive = re.search(r"drive\.google\.com/file/d/([^/]+)", url)
    if drive:
        return {
            "kind": "drive",
            "name": note or "Vídeo no Drive",
            "id": drive.group(1),
            "url": url,
        }
    return {"kind": "link", "name": note or link_label(url), "url": url}


def build_drive_projects() -> list[dict]:
    entries = parse_drive_folder()
    projects = []
    used: set[str] = set()
    links_id = ""
    for file_id, name in entries:
        if name.lower().startswith("links projetos"):
            links_id = file_id
            continue
        item_id = unique_id(f"drive--{slugify(name)}", used)
        projects.append(
            {
                "id": item_id,
                "source": "drive",
                "edition": "drive",
                "editionLabel": "Vídeos no Drive",
                "title": re.sub(r"\.[A-Za-z0-9]+$", "", name),
                "author": "",
                "summary": "",
                "objectives": "",
                "resources": "",
                "folder": "",
                "githubUrl": "",
                "presentations": [
                    {
                        "kind": "drive",
                        "name": name,
                        "id": file_id,
                        "url": f"https://drive.google.com/file/d/{file_id}/view",
                    }
                ],
                "documents": [],
                "code": [],
                "images": [],
            }
        )
    if links_id:
        for item in parse_links_doc(links_id):
            presentations = []
            for url in item["urls"]:
                presentations.append(presentation_from_url(url))
            note = " ".join(item["notes"]).strip()
            item_id = unique_id(f"drive--{slugify(item['title'])}", used)
            projects.append(
                {
                    "id": item_id,
                    "source": "drive",
                    "edition": "ligacoes",
                    "editionLabel": "Ligações externas",
                    "title": item["title"],
                    "author": "",
                    "summary": "",
                    "objectives": note,
                    "resources": "",
                    "folder": "",
                    "githubUrl": "",
                    "presentations": presentations,
                    "documents": [],
                    "code": [],
                    "images": [],
                }
            )
    projects.sort(key=lambda item: item["title"].lower())
    return projects


def load_overrides() -> dict[str, str]:
    path = SITE / "data" / "objetivos.json"
    if not path.exists():
        return {}
    data = json.loads(path.read_text(encoding="utf-8"))
    return {str(key): str(value).strip() for key, value in data.items() if str(value).strip()}


def main() -> None:
    print("A ler a árvore do GitHub...")
    tree = load_tree()
    print("A ler os README dos projetos...")
    projects = build_repo_projects(tree)
    print(f"  {len(projects)} projetos no repositório")
    print("A ler a pasta do Google Drive...")
    drive_projects = build_drive_projects()
    print(f"  {len(drive_projects)} vídeos e ligações")
    overrides = load_overrides()
    for project in projects:
        override = overrides.get(project["folder"]) or overrides.get(project["id"])
        if override:
            project["objectives"] = override
            project["objectivesSource"] = "manual"
        elif project["objectives"] and not project.get("objectivesSource"):
            project["objectivesSource"] = "readme"
    for project in drive_projects:
        project["objectivesSource"] = "drive" if project["objectives"] else ""

    editions = []
    seen = []
    for project in projects:
        if project["edition"] not in seen:
            seen.append(project["edition"])
            editions.append({"id": project["edition"], "label": project["editionLabel"]})
    if any(item["edition"] == "drive" for item in drive_projects):
        editions.append({"id": "drive", "label": "Vídeos no Drive"})
    if any(item["edition"] == "ligacoes" for item in drive_projects):
        editions.append({"id": "ligacoes", "label": "Ligações externas"})

    catalog = {
        "generatedAt": datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ"),
        "training": "Programação com Arduinos",
        "organization": "ANPRI",
        "repo": REPO,
        "branch": BRANCH,
        "repoUrl": f"https://github.com/{REPO}",
        "driveUrl": f"https://drive.google.com/drive/folders/{DRIVE_FOLDER}",
        "editions": editions,
        "projects": projects + drive_projects,
    }
    OUT_JS.parent.mkdir(parents=True, exist_ok=True)
    payload = json.dumps(catalog, ensure_ascii=False, indent=2)
    OUT_JSON.write_text(payload + "\n", encoding="utf-8")
    OUT_JS.write_text("window.CATALOG = " + payload + ";\n", encoding="utf-8")
    with_text = sum(1 for item in catalog["projects"] if item.get("summary") or item.get("objectives"))
    with_code = sum(1 for item in catalog["projects"] if item["code"])
    with_video = sum(
        1
        for item in catalog["projects"]
        if any(p["kind"] in {"youtube", "video", "drive"} for p in item["presentations"])
    )
    print(f"Catálogo escrito em {OUT_JS}")
    print(
        f"  projetos={len(catalog['projects'])} com texto={with_text} codigo={with_code} video={with_video}"
    )
    print("\nPropostas com objetivos ou enquadramento:")
    for item in projects:
        if item.get("summary") or item.get("objectivesSource") == "proposta":
            print(f"- {item['title']} [{item['objectivesSource']}]")


if __name__ == "__main__":
    main()
