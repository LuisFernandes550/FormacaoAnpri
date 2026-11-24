const catalog = window.CATALOG;
const view = document.getElementById("view");
const search = document.getElementById("q");

const state = {
  query: "",
  edition: "todas",
  video: false,
  code: false,
  text: false,
  view: "list",
};

let shownId = "";
let loadedCode = "";

function esc(value) {
  return String(value ?? "").replace(/[&<>"']/g, (char) => ({
    "&": "&amp;",
    "<": "&lt;",
    ">": "&gt;",
    '"': "&quot;",
    "'": "&#39;",
  }[char]));
}

function linkify(escaped) {
  return escaped.replace(/https?:\/\/[^\s<]+/g, (url) => {
    return `<a href="${url}" target="_blank" rel="noopener">${url}</a>`;
  });
}

function richText(text) {
  const lines = String(text || "").split(/\n/).map((line) => line.trim()).filter(Boolean);
  let html = "";
  let items = [];
  const flush = () => {
    if (!items.length) return;
    html += `<ul>${items.map((item) => `<li>${linkify(esc(item))}</li>`).join("")}</ul>`;
    items = [];
  };
  for (const line of lines) {
    const bullet = line.match(/^[-•–]\s+(.+)/);
    if (bullet) {
      items.push(bullet[1]);
      continue;
    }
    flush();
    html += `<p>${linkify(esc(line))}</p>`;
  }
  flush();
  return html;
}

function formatSize(bytes) {
  if (!bytes) return "";
  if (bytes < 1024) return `${bytes} B`;
  if (bytes < 1024 * 1024) return `${Math.round(bytes / 1024)} KB`;
  return `${(bytes / 1024 / 1024).toFixed(1)} MB`;
}

function hasVideo(project) {
  return project.presentations.some((item) => ["youtube", "video", "drive"].includes(item.kind));
}

function hasSlides(project) {
  return project.presentations.some((item) => ["slides", "pdf"].includes(item.kind));
}

function hasText(project) {
  return Boolean(project.summary || project.objectives);
}

function matches(project) {
  if (state.edition !== "todas" && project.edition !== state.edition) return false;
  if (state.video && !hasVideo(project)) return false;
  if (state.code && project.code.length === 0) return false;
  if (state.text && !hasText(project)) return false;
  const query = state.query.trim().toLowerCase();
  if (!query) return true;
  const haystack = [
    project.title,
    project.author,
    project.editionLabel,
    project.summary,
    project.objectives,
    project.folder,
  ].join("\n").toLowerCase();
  return haystack.includes(query);
}

function filtered() {
  return catalog.projects.filter(matches);
}

function pageFromHash() {
  const hash = location.hash || "#/";
  if (hash.startsWith("#/turmas")) return "turmas";
  if (hash.startsWith("#/sobre")) return "sobre";
  return "projetos";
}

function currentId() {
  const match = location.hash.match(/^#\/p\/([^/?]+)/);
  return match ? decodeURIComponent(match[1]) : "";
}

function isNarrow() {
  return window.matchMedia("(max-width: 980px)").matches;
}

function icon(name) {
  const paths = {
    code: '<path d="M8 8L4 12l4 4M16 8l4 4-4 4" fill="none" stroke="currentColor" stroke-width="1.7" stroke-linecap="round" stroke-linejoin="round"/>',
    play: '<circle cx="12" cy="12" r="8.5" fill="none" stroke="currentColor" stroke-width="1.8"/><path d="M10.2 8.3v7.4l6.2-3.7z" fill="currentColor"/>',
    users: '<circle cx="9" cy="9" r="2.2" fill="none" stroke="currentColor" stroke-width="1.6"/><circle cx="15.5" cy="10" r="1.7" fill="none" stroke="currentColor" stroke-width="1.6"/><path d="M4.8 17.2c.6-2 2.3-3.2 4.2-3.2s3.6 1.2 4.2 3.2M14 14.2c.8-.5 1.8-.7 2.6-.5 1.3.3 2.2 1.4 2.6 2.8" fill="none" stroke="currentColor" stroke-width="1.6" stroke-linecap="round"/>',
    video: '<rect x="3.5" y="6.5" width="12" height="11" rx="2" fill="none" stroke="currentColor" stroke-width="1.7"/><path d="M15.5 10.5l5-2.5v8l-5-2.5z" fill="none" stroke="currentColor" stroke-width="1.7" stroke-linejoin="round"/>',
    person: '<circle cx="12" cy="8" r="3" fill="none" stroke="currentColor" stroke-width="1.7"/><path d="M6 18.5c.8-2.6 2.8-4 6-4s5.2 1.4 6 4" fill="none" stroke="currentColor" stroke-width="1.7" stroke-linecap="round"/>',
    screen: '<rect x="4" y="5" width="16" height="11" rx="1.6" fill="none" stroke="currentColor" stroke-width="1.7"/><path d="M9 19.5h6M12 16v3.5" fill="none" stroke="currentColor" stroke-width="1.7" stroke-linecap="round"/>',
    target: '<circle cx="12" cy="12" r="7" fill="none" stroke="currentColor" stroke-width="1.7"/><circle cx="12" cy="12" r="3" fill="none" stroke="currentColor" stroke-width="1.7"/><circle cx="12" cy="12" r="1" fill="currentColor"/>',
    folder: '<path d="M3.5 8.5h6l1.5-2h9.5v11h-17z" fill="none" stroke="currentColor" stroke-width="1.7" stroke-linejoin="round"/>',
    external: '<path d="M9 7H6.5A1.5 1.5 0 005 8.5v9A1.5 1.5 0 006.5 19h9a1.5 1.5 0 001.5-1.5V15" fill="none" stroke="currentColor" stroke-width="1.7" stroke-linecap="round"/><path d="M12 5h7v7M19 5l-8 8" fill="none" stroke="currentColor" stroke-width="1.7" stroke-linecap="round" stroke-linejoin="round"/>',
    copy: '<rect x="8" y="8" width="10" height="11" rx="1.5" fill="none" stroke="currentColor" stroke-width="1.7"/><path d="M6 15.5H5.5A1.5 1.5 0 014 14V5.5A1.5 1.5 0 015.5 4H14a1.5 1.5 0 011.5 1.5V6" fill="none" stroke="currentColor" stroke-width="1.7"/>',
    grid: '<rect x="4" y="4" width="6.5" height="6.5" rx="1.2" fill="none" stroke="currentColor" stroke-width="1.7"/><rect x="13.5" y="4" width="6.5" height="6.5" rx="1.2" fill="none" stroke="currentColor" stroke-width="1.7"/><rect x="4" y="13.5" width="6.5" height="6.5" rx="1.2" fill="none" stroke="currentColor" stroke-width="1.7"/><rect x="13.5" y="13.5" width="6.5" height="6.5" rx="1.2" fill="none" stroke="currentColor" stroke-width="1.7"/>',
    list: '<path d="M8 7h12M8 12h12M8 17h12M4.5 7h.01M4.5 12h.01M4.5 17h.01" fill="none" stroke="currentColor" stroke-width="1.8" stroke-linecap="round"/>',
  };
  return `<svg viewBox="0 0 24 24" width="18" height="18" aria-hidden="true">${paths[name] || ""}</svg>`;
}

function imageScore(image) {
  const name = image.name.toLowerCase();
  let score = 0;
  if (/\.jpe?g$/.test(name)) score += 4;
  if (/img_|dsc_|foto|photo|maquete|vista/.test(name)) score += 3;
  if (/esquema|circuito|fase|fritz|diagrama|liga/.test(name)) score -= 4;
  return score;
}

function bestImage(images) {
  if (!images || !images.length) return null;
  return [...images].sort((a, b) => imageScore(b) - imageScore(a) || (b.size || 0) - (a.size || 0))[0];
}

function geniallyCover(url) {
  const match = String(url).match(/genially\.com\/([a-f0-9]{24})|genial\.ly\/([a-f0-9]{24})/i);
  const id = match && (match[1] || match[2]);
  return id ? `https://genially-thumbnails.genially.com/${id}.png` : "";
}

function coverUrl(project, large) {
  const youtube = project.presentations.find((item) => item.kind === "youtube");
  if (youtube) {
    const quality = large ? "hqdefault" : "mqdefault";
    return `https://i.ytimg.com/vi/${youtube.id}/${quality}.jpg`;
  }
  const photo = bestImage(project.images);
  if (photo) return photo.cdnUrl;
  const drive = project.presentations.find((item) => item.kind === "drive");
  if (drive) return `https://lh3.googleusercontent.com/d/${drive.id}=${large ? "w800" : "w400"}`;
  const genial = project.presentations.find((item) => item.kind === "link" && /genial/i.test(item.url || ""));
  if (genial) {
    const fromLink = geniallyCover(genial.url);
    if (fromLink) return fromLink;
  }
  if (project.id === "drive--iot-smart-home") return "https://escolaonline.pt/arduino/img/ligacoes.png";
  return "";
}

function hue(project) {
  return [...project.id].reduce((sum, char) => sum + char.charCodeAt(0), 0) % 360;
}

function thumb(project) {
  const src = coverUrl(project, false);
  if (!src) {
    return `<span class="thumb"><span class="fallback" style="--hue:${hue(project)}">${esc(project.title)}</span></span>`;
  }
  return `<span class="thumb"><img src="${esc(src)}" alt=""></span>`;
}

function marks(project) {
  const items = [];
  if (hasVideo(project)) items.push(`<span class="mark" title="Vídeo">${icon("play")}</span>`);
  if (hasSlides(project)) items.push(`<span class="mark" title="Apresentação">${icon("screen")}</span>`);
  if (project.code.length) items.push(`<span class="mark" title="Código">${icon("code")}</span>`);
  return `<span class="marks">${items.join("")}</span>`;
}

function authorLine(project, prefix) {
  if (!project.author) return "";
  const label = prefix ? `${prefix} ${project.author}` : project.author;
  return `<p class="author">${icon("person")}<span>${esc(label)}</span></p>`;
}

function go(href) {
  if (location.hash === href) {
    render();
    return;
  }
  history.pushState(null, "", href);
  render();
}

function syncNav() {
  const page = pageFromHash();
  document.querySelectorAll("[data-nav]").forEach((link) => {
    if (link.dataset.nav === page) link.setAttribute("aria-current", "page");
    else link.removeAttribute("aria-current");
  });
}

function heroArt() {
  return `
    <svg viewBox="0 0 640 420" role="img" aria-label="">
      <defs>
        <radialGradient id="glow" cx="50%" cy="50%" r="50%">
          <stop offset="0%" stop-color="#7ee7ff" stop-opacity="0.95"/>
          <stop offset="55%" stop-color="#2f8cff" stop-opacity="0.35"/>
          <stop offset="100%" stop-color="#2f8cff" stop-opacity="0"/>
        </radialGradient>
        <linearGradient id="pcb" x1="0" y1="0" x2="1" y2="1">
          <stop offset="0%" stop-color="#1c78b8"/>
          <stop offset="100%" stop-color="#0c4f86"/>
        </linearGradient>
        <filter id="soft" x="-30%" y="-30%" width="160%" height="160%">
          <feGaussianBlur stdDeviation="8"/>
        </filter>
      </defs>
      <ellipse cx="390" cy="210" rx="210" ry="150" fill="url(#glow)" filter="url(#soft)"/>
      <path d="M80 250c80-90 150-40 210-110" fill="none" stroke="#7ee7ff" stroke-width="3" stroke-linecap="round" opacity="0.7"/>
      <path d="M120 300c120-40 180-20 280-130" fill="none" stroke="#38bdf8" stroke-width="2" stroke-linecap="round" opacity="0.45"/>
      <g transform="translate(168 78) rotate(-8 180 120)">
        <rect x="0" y="28" width="360" height="230" rx="16" fill="url(#pcb)" stroke="#9ad7ff" stroke-opacity="0.35"/>
        <rect x="-18" y="92" width="28" height="58" rx="4" fill="#d7e4ef"/>
        <rect x="338" y="150" width="34" height="46" rx="6" fill="#c5d4e4"/>
        <rect x="118" y="78" width="92" height="92" rx="6" fill="#111827"/>
        <g fill="#d6b25e">
          ${Array.from({ length: 14 }, (_, i) => `<rect x="${28 + i * 18}" y="40" width="8" height="14" rx="1"/>`).join("")}
          ${Array.from({ length: 14 }, (_, i) => `<rect x="${28 + i * 18}" y="228" width="8" height="14" rx="1"/>`).join("")}
        </g>
        <g fill="#67e8f9">
          <circle cx="70" cy="150" r="5"/>
          <circle cx="250" cy="120" r="4"/>
          <circle cx="286" cy="168" r="4"/>
        </g>
        <circle cx="300" cy="70" r="16" fill="#e8eef5" stroke="#94a3b8"/>
        <text x="168" y="132" text-anchor="middle" fill="#7dd3fc" font-family="Inter, sans-serif" font-size="18" font-weight="700">Arduino</text>
      </g>
    </svg>`;
}

function stats() {
  const repoCount = catalog.projects.filter((project) => project.source === "github").length;
  const withCode = catalog.projects.filter((project) => project.code.length).length;
  const withVideo = catalog.projects.filter(hasVideo).length;
  const turmas = catalog.editions.filter((edition) => edition.id.startsWith("20")).length;
  const items = [
    ["code", repoCount, "Projetos"],
    ["code", withCode, "Com código"],
    ["play", withVideo, "Com vídeo"],
    ["users", turmas, "Turmas"],
  ];
  return items.map(([name, value, label]) => `
    <div class="stat">${icon(name)}<div><strong>${value}</strong><span>${label}</span></div></div>
  `).join("");
}

function turmaEditions() {
  return catalog.editions.filter((edition) => edition.id !== "ligacoes");
}

function filterBar(count) {
  const chips = [
    `<button type="button" class="chip" data-edition="todas" aria-pressed="${state.edition === "todas"}">Todas</button>`,
    ...turmaEditions().map((edition) => `
      <button type="button" class="chip" data-edition="${esc(edition.id)}" aria-pressed="${state.edition === edition.id}">${esc(edition.label)}</button>
    `),
  ].join("");
  return `
    <div class="filter-card">
      <div class="chips" role="group" aria-label="Turma">${chips}</div>
      <div class="filter-row">
        <div class="toggles">
          <select id="link-filter" class="select" aria-label="Ligações externas">
            <option value="" ${state.edition === "ligacoes" ? "" : "selected"}>Ligações externas</option>
            <option value="ligacoes" ${state.edition === "ligacoes" ? "selected" : ""}>Apenas ligações externas</option>
          </select>
          <button type="button" class="toggle" data-flag="video" aria-pressed="${state.video}">${icon("play")} Com vídeo</button>
          <button type="button" class="toggle" data-flag="code" aria-pressed="${state.code}">${icon("code")} Com código</button>
          <button type="button" class="toggle" data-flag="text" aria-pressed="${state.text}">${icon("target")} Com objetivos</button>
        </div>
        <div class="filter-end">
          <span id="count">${count} projetos</span>
          <div class="view-switch" role="group" aria-label="Modo de vista">
            <button type="button" class="view-btn" data-view="grid" aria-pressed="${state.view === "grid"}" title="Grelha">${icon("grid")}</button>
            <button type="button" class="view-btn" data-view="list" aria-pressed="${state.view === "list"}" title="Lista">${icon("list")}</button>
          </div>
        </div>
      </div>
    </div>`;
}

function rowHtml(project, selected) {
  const current = selected && project.id === selected.id ? ' aria-current="true"' : "";
  return `
    <a class="row" href="#/p/${encodeURIComponent(project.id)}"${current}>
      ${thumb(project)}
      <span class="row-copy">
        <p class="edition">${esc(project.editionLabel)}</p>
        <h2>${esc(project.title)}</h2>
        ${authorLine(project)}
      </span>
      ${marks(project)}
    </a>`;
}

function cardHtml(project) {
  return `
    <a class="card" href="#/p/${encodeURIComponent(project.id)}">
      ${thumb(project)}
      <span class="card-copy">
        <p class="edition">${esc(project.editionLabel)}</p>
        <h2>${esc(project.title)}</h2>
        ${authorLine(project)}
      </span>
      ${marks(project)}
    </a>`;
}

function pick(projects) {
  const id = currentId();
  const found = projects.find((project) => project.id === id);
  if (found) return found;
  if (isNarrow()) return null;
  return projects[0] || null;
}

function paintList(projects, selected) {
  const list = document.getElementById("list");
  if (!list) return;
  if (!projects.length) {
    list.innerHTML = `<p class="empty">Nenhum projeto corresponde a esta pesquisa.</p>`;
    return;
  }
  if (state.view === "grid") {
    list.innerHTML = `<div class="card-grid">${projects.map(cardHtml).join("")}</div>`;
    return;
  }
  list.innerHTML = projects.map((project) => rowHtml(project, selected)).join("");
}

function player(item, label) {
  if (item.kind === "youtube") {
    return `
      <figure class="stack">
        <div class="player">
          <iframe src="https://www.youtube-nocookie.com/embed/${esc(item.id)}" title="${esc(label)}" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen loading="lazy"></iframe>
        </div>
        <figcaption class="caption">${icon("play")}<a href="${esc(item.url)}" target="_blank" rel="noopener">${esc(label)}</a></figcaption>
      </figure>`;
  }
  if (item.kind === "drive") {
    return `
      <figure class="stack">
        <div class="player">
          <iframe src="https://drive.google.com/file/d/${esc(item.id)}/preview" title="${esc(item.name || label)}" allow="autoplay" allowfullscreen loading="lazy"></iframe>
        </div>
        <figcaption class="caption"><a href="${esc(item.url)}" target="_blank" rel="noopener">${esc(item.name || "Abrir no Drive")}</a></figcaption>
      </figure>`;
  }
  if (item.kind === "video") {
    const size = formatSize(item.size);
    if (item.size && item.size > 30 * 1024 * 1024) {
      return `<p><a href="${esc(item.url)}" target="_blank" rel="noopener">${esc(item.name)}</a> <span class="note">${esc(size)} · o vídeo é grande e abre no GitHub</span></p>`;
    }
    return `
      <figure class="stack">
        <div class="player"><video controls preload="metadata" src="${esc(item.rawUrl)}"></video></div>
        <figcaption class="caption">${esc(item.name)}${size ? ` · ${esc(size)}` : ""}</figcaption>
      </figure>`;
  }
  return "";
}

function fileLink(item) {
  const size = formatSize(item.size);
  const href = item.url || item.rawUrl;
  return `<li><a href="${esc(href)}" target="_blank" rel="noopener">${esc(item.name)}</a>${size ? ` <span class="note">${esc(size)}</span>` : ""}</li>`;
}

function renderDetail(project) {
  const detail = document.getElementById("detail");
  if (!detail || !project) return;
  if (shownId === project.id && detail.querySelector(".detail-head")) return;
  shownId = project.id;
  loadedCode = "";

  const slides = project.presentations.filter((item) => item.kind === "slides");
  const pdfs = project.presentations.filter((item) => item.kind === "pdf");
  const links = project.presentations.filter((item) => item.kind === "link");
  const media = project.presentations.filter((item) => ["youtube", "drive", "video"].includes(item.kind));
  let videoIndex = 0;
  const mediaHtml = media.map((item) => {
    videoIndex += 1;
    const label = item.kind === "youtube" && media.filter((entry) => entry.kind === "youtube").length === 1
      ? "Vídeo da apresentação"
      : (media.length > 1 ? `Vídeo ${videoIndex}` : "Vídeo da apresentação");
    return player(item, label);
  }).join("");

  const textBlocks = [];
  if (project.summary) textBlocks.push(richText(project.summary));
  if (project.objectives) {
    textBlocks.push(`<h4 class="subhead">Objetivos</h4>${richText(project.objectives)}`);
  }
  if (project.resources) {
    textBlocks.push(`<h4 class="subhead">Recursos</h4>${richText(project.resources)}`);
  }
  const objectivesHtml = textBlocks.length
    ? `<div class="prose">${textBlocks.join("")}</div>`
    : `<p class="note">Os objetivos deste projeto ainda não estão descritos no repositório.</p>`;

  const docs = [...slides, ...pdfs, ...(project.documents || [])];
  const docsHtml = docs.length ? `<ul class="doc-list">${docs.map(fileLink).join("")}</ul>` : "";
  const linksHtml = links.length
    ? `<ul class="link-list">${links.map((item) => `<li><a href="${esc(item.url)}" target="_blank" rel="noopener">${esc(item.name)}</a></li>`).join("")}</ul>`
    : "";
  const cover = coverUrl(project, true);
  const gallery = (project.images || []).filter((image) => image.cdnUrl !== cover).slice(0, 6);
  const galleryHtml = gallery.length
    ? `<div class="gallery">${gallery.map((image) => `<a href="${esc(image.rawUrl)}" target="_blank" rel="noopener"><img src="${esc(image.cdnUrl)}" alt="${esc(image.name)}"></a>`).join("")}</div>`
    : "";
  const presentationBody = [mediaHtml, docsHtml, linksHtml, galleryHtml].filter(Boolean).join("");
  const presentationHtml = presentationBody
    ? presentationBody
    : `<p class="note">Ainda não há apresentação associada a este projeto.</p>`;

  const codeHtml = project.code.length
    ? `
      <div class="file-tabs" role="tablist">
        ${project.code.map((file, index) => `<button type="button" class="file-btn" role="tab" data-file="${index}" aria-selected="${index === 0}">${esc(file.name)}</button>`).join("")}
      </div>
      <div class="codebox"><pre><code id="code-view">A carregar o código…</code></pre></div>`
    : `<p class="note">Ainda não há código deste projeto no repositório. ${project.githubUrl ? `<a href="${esc(project.githubUrl)}" target="_blank" rel="noopener">Abrir a pasta</a>` : ""}</p>`;

  const external = project.githubUrl
    ? `<a class="gh-link" href="${esc(project.githubUrl)}" target="_blank" rel="noopener">${icon("folder")} Pasta no GitHub ${icon("external")}</a>`
    : "";

  detail.innerHTML = `
    <a class="back" href="#/">← Projetos</a>
    <header class="detail-head">
      ${thumb(project)}
      <div class="detail-copy">
        <p class="edition">${esc(project.editionLabel)}</p>
        <h2>${esc(project.title)}</h2>
        ${authorLine(project, "Formando:")}
      </div>
      ${external}
    </header>
    <section class="block">
      <div class="block-head"><h3><span class="ico">${icon("target")}</span> Designação e objetivos</h3></div>
      ${objectivesHtml}
    </section>
    <section class="block">
      <div class="block-head"><h3><span class="ico">${icon("play")}</span> Apresentação</h3></div>
      <div class="stack">${presentationHtml}</div>
    </section>
    <section class="block">
      <div class="block-head">
        <h3><span class="ico">${icon("code")}</span> Código</h3>
        ${project.code.length ? `<button type="button" class="text-btn" id="copy-code">${icon("copy")} <span>Copiar</span></button>` : ""}
      </div>
      ${codeHtml}
    </section>`;

  document.title = `${project.title} · Programação com Arduinos`;
  document.querySelector(".detail-head")?.scrollIntoView({ block: "nearest" });
  if (project.code.length) loadCode(project, 0);
}

function applyWorkspace(projects, selected) {
  const workspace = document.querySelector(".workspace");
  if (!workspace) return;
  const showDetail = state.view === "list" && Boolean(selected) && (!isNarrow() || Boolean(currentId()));
  workspace.classList.toggle("is-grid", state.view === "grid");
  workspace.classList.toggle("has-project", showDetail);
  paintList(projects, state.view === "list" ? selected : null);
  const detail = document.getElementById("detail");
  if (state.view === "grid" || !selected) {
    if (detail && state.view === "grid") detail.innerHTML = "";
    if (!selected) {
      shownId = "";
      if (detail) detail.innerHTML = "";
    }
    document.title = "Catálogo · Programação com Arduinos";
    return;
  }
  renderDetail(selected);
}

function refreshResults() {
  const projects = filtered();
  const count = document.getElementById("count");
  if (count) count.textContent = `${projects.length} projetos`;
  const selected = pick(projects);
  if (selected && selected.id !== currentId() && !isNarrow()) {
    history.replaceState(null, "", `#/p/${encodeURIComponent(selected.id)}`);
  }
  applyWorkspace(projects, selected);
}

function renderProjetos() {
  shownId = "";
  const projects = filtered();
  view.innerHTML = `
    <section class="hero">
      <div class="wrap hero-grid">
        <div>
          <p class="eyebrow">PROJETOS DOS FORMANDOS</p>
          <h1>Programação com <span>Arduinos</span></h1>
          <p class="hero-lead">Repositório de projetos desenvolvidos pelos formandos, com código, vídeo, apresentação e objetivos.</p>
          <div class="stats">${stats()}</div>
        </div>
        <div class="hero-art">${heroArt()}</div>
      </div>
    </section>
    <div class="wrap panel">
      ${filterBar(projects.length)}
      <div class="workspace">
        <div class="list-pane" id="list"></div>
        <div class="detail-pane" id="detail"></div>
      </div>
    </div>`;
  applyWorkspace(projects, pick(projects));
}

function renderTurmas() {
  shownId = "";
  document.title = "Turmas · Programação com Arduinos";
  const cards = catalog.editions.map((edition) => {
    const projects = catalog.projects.filter((project) => project.edition === edition.id);
    const withCode = projects.filter((project) => project.code.length).length;
    return `
      <a class="turma-card" href="#/" data-edition="${esc(edition.id)}">
        <strong>${esc(edition.label)}</strong>
        <p>${projects.length} projetos · ${withCode} com código</p>
      </a>`;
  }).join("");
  view.innerHTML = `
    <div class="wrap page">
      <h1>Turmas</h1>
      <p class="page-lead">Cada turma corresponde a uma edição da formação. Escolhe uma para ver só esses projetos.</p>
      <div class="turma-grid">${cards}</div>
    </div>`;
}

function renderSobre() {
  shownId = "";
  document.title = "Sobre o projeto · Programação com Arduinos";
  const repo = catalog.repoUrl || "https://github.com/LuisFernandes550/FormacaoAnpri";
  const drive = catalog.driveUrl || "https://drive.google.com/drive/folders/1S66_71QPppwhJrDpkIF-GApYYr0ZmdBV";
  view.innerHTML = `
    <div class="wrap page">
      <h1>Sobre o projeto</h1>
      <p class="page-lead">Catálogo dos projetos feitos na formação Programação com Arduinos, da ANPRI.</p>
      <article class="page-card">
        <h2>O que mostra cada projeto</h2>
        <p>A designação e os objetivos, quando estão escritos no repositório ou na proposta. A apresentação, em vídeo, diapositivos ou PDF. O código dos sketches que os formandos entregaram.</p>
      </article>
      <article class="page-card">
        <h2>De onde vêm os projetos</h2>
        <p>Os trabalhos das turmas de 2022 a 2024 estão no <a href="${esc(repo)}" target="_blank" rel="noopener">repositório FormacaoAnpri</a>. Os vídeos e as ligações mais antigas estão na <a href="${esc(drive)}" target="_blank" rel="noopener">pasta do Google Drive</a>.</p>
      </article>
    </div>`;
}

function render() {
  if (!catalog || !catalog.projects) {
    view.innerHTML = `<p class="empty">Não foi possível ler o catálogo.</p>`;
    return;
  }
  syncNav();
  const page = pageFromHash();
  if (page === "turmas") renderTurmas();
  else if (page === "sobre") renderSobre();
  else renderProjetos();
}

async function loadCode(project, index) {
  const file = project.code[index];
  const node = document.getElementById("code-view");
  if (!node || !file) return;
  document.querySelectorAll(".file-btn").forEach((button, buttonIndex) => {
    button.setAttribute("aria-selected", String(buttonIndex === index));
  });
  if (file.size && file.size > 120000) {
    loadedCode = "";
    node.textContent = "Este ficheiro é demasiado grande para mostrar aqui.";
    return;
  }
  node.textContent = "A carregar o código…";
  try {
    const response = await fetch(file.cdnUrl);
    if (!response.ok) throw new Error(String(response.status));
    loadedCode = await response.text();
    const language = file.language || "cpp";
    let html = loadedCode;
    if (window.hljs) {
      const result = window.hljs.highlight(loadedCode, { language, ignoreIllegals: true });
      html = result.value;
    } else {
      html = esc(loadedCode);
    }
    node.innerHTML = html.split("\n").map((line) => `<span class="ln">${line || " "}</span>`).join("");
    node.className = `language-${language}`;
  } catch (error) {
    loadedCode = "";
    node.textContent = "Não foi possível mostrar o código nesta página. Abre o ficheiro no GitHub.";
  }
}

search.addEventListener("input", () => {
  state.query = search.value;
  if (pageFromHash() !== "projetos") {
    go("#/");
    return;
  }
  refreshResults();
});

document.addEventListener("click", async (event) => {
  const nav = event.target.closest("[data-nav]");
  if (nav) {
    event.preventDefault();
    go(nav.getAttribute("href"));
    return;
  }

  const fileButton = event.target.closest("[data-file]");
  if (fileButton) {
    const project = catalog.projects.find((item) => item.id === shownId);
    if (project) loadCode(project, Number(fileButton.dataset.file));
    return;
  }

  const copy = event.target.closest("#copy-code");
  if (copy) {
    if (!loadedCode) return;
    try {
      await navigator.clipboard.writeText(loadedCode);
      const label = copy.querySelector("span");
      label.textContent = "Copiado";
      setTimeout(() => { label.textContent = "Copiar"; }, 1200);
    } catch (error) {
      copy.querySelector("span").textContent = "Não copiou";
    }
    return;
  }

  const edition = event.target.closest("button[data-edition]");
  if (edition) {
    state.edition = edition.dataset.edition;
    if (pageFromHash() !== "projetos") go("#/");
    else render();
    return;
  }

  const flag = event.target.closest("[data-flag]");
  if (flag) {
    state[flag.dataset.flag] = flag.getAttribute("aria-pressed") !== "true";
    render();
    return;
  }

  const mode = event.target.closest("[data-view]");
  if (mode) {
    state.view = mode.dataset.view;
    if (state.view === "list" && isNarrow() && currentId()) {
      history.pushState(null, "", "#/");
    }
    render();
    return;
  }

  const jump = event.target.closest("a[href^='#/']");
  if (jump && event.button === 0 && !event.metaKey && !event.ctrlKey && !event.shiftKey && !event.altKey) {
    event.preventDefault();
    if (jump.dataset.edition) state.edition = jump.dataset.edition;
    if (state.view === "grid" && jump.classList.contains("card")) state.view = "list";
    go(jump.getAttribute("href"));
  }
});

document.addEventListener("change", (event) => {
  if (event.target.id !== "link-filter") return;
  if (event.target.value === "ligacoes") state.edition = "ligacoes";
  else if (state.edition === "ligacoes") state.edition = "todas";
  render();
});

window.addEventListener("popstate", render);

let resizeTimer = 0;
window.addEventListener("resize", () => {
  clearTimeout(resizeTimer);
  resizeTimer = setTimeout(() => {
    if (pageFromHash() === "projetos" && document.getElementById("list")) refreshResults();
  }, 150);
});

render();
