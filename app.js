const catalog = window.CATALOG;
const view = document.getElementById("view");
const toolbar = document.getElementById("toolbar");

const state = {
  query: "",
  edition: "todas",
  video: false,
  code: false,
  text: false,
};

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

function currentId() {
  const match = location.hash.match(/^#\/p\/([^/]+)$/);
  return match ? decodeURIComponent(match[1]) : "";
}

function findProject(id) {
  return catalog.projects.find((project) => project.id === id);
}

function shortEdition(project) {
  if (project.edition === "drive") return "Drive";
  if (project.edition === "ligacoes") return "Ligações";
  return project.edition.replace("_", " ");
}

function marks(project) {
  const items = [];
  if (hasVideo(project)) items.push("vídeo");
  if (hasSlides(project)) items.push("apresentação");
  if (project.code.length) items.push("código");
  return items.join(" · ");
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

function coverTag(project, className, large) {
  const src = coverUrl(project, large);
  if (!src) {
    return `<span class="${className} cover-fallback" style="--hue:${hue(project)}"><span>${esc(project.title)}</span></span>`;
  }
  return `<img class="${className}" src="${esc(src)}" alt="" loading="lazy">`;
}

function markTags(project) {
  const items = [];
  if (hasVideo(project)) items.push("Vídeo");
  if (hasSlides(project)) items.push("Apresentação");
  if (project.code.length) items.push("Código");
  return items.map((item) => `<li>${item}</li>`).join("");
}

function renderToolbar() {
  const repoCount = catalog.projects.filter((project) => project.source === "github").length;
  const withCode = catalog.projects.filter((project) => project.code.length).length;
  const withVideo = catalog.projects.filter(hasVideo).length;
  const editionChips = [
    `<button type="button" class="chip" data-edition="todas" aria-pressed="${state.edition === "todas"}">Todas</button>`,
    ...catalog.editions.map((edition) => {
      const pressed = state.edition === edition.id;
      return `<button type="button" class="chip" data-edition="${esc(edition.id)}" aria-pressed="${pressed}">${esc(edition.label)}</button>`;
    }),
  ].join("");

  toolbar.innerHTML = `
    <p class="stats">${repoCount} projetos no repositório · ${withCode} com código · ${withVideo} com vídeo · ${catalog.editions.filter((edition) => edition.id.startsWith("20")).length} turmas</p>
    <div class="controls">
      <label class="search">Pesquisar por nome, formando ou objetivo
        <input id="q" type="search" value="${esc(state.query)}" placeholder="Ex.: estufa, estacionamento, Vera">
      </label>
      <div class="chips" role="group" aria-label="Turma">${editionChips}</div>
      <div class="toggles">
        <button type="button" class="toggle" data-flag="video" aria-pressed="${state.video}">Com vídeo</button>
        <button type="button" class="toggle" data-flag="code" aria-pressed="${state.code}">Com código</button>
        <button type="button" class="toggle" data-flag="text" aria-pressed="${state.text}">Com objetivos</button>
      </div>
    </div>
    <p class="result-line" id="result-line"></p>
  `;
  document.getElementById("q").addEventListener("input", (event) => {
    state.query = event.target.value;
    if (currentId()) location.hash = "#/";
    else renderList();
  });
}

function renderList() {
  const projects = catalog.projects.filter(matches);
  const line = document.getElementById("result-line");
  if (line) {
    line.textContent = projects.length === catalog.projects.length
      ? `${projects.length} projetos`
      : `${projects.length} de ${catalog.projects.length} projetos`;
  }
  if (!projects.length) {
    view.innerHTML = `<p class="empty">Nenhum projeto corresponde a esta pesquisa.</p>`;
    return;
  }
  view.innerHTML = `<div class="grid">${projects.map((project) => `
    <a class="card" href="#/p/${encodeURIComponent(project.id)}">
      <div class="card-media">
        ${coverTag(project, "card-img", true)}
        <span class="badge">${esc(project.editionLabel)}</span>
      </div>
      <div class="card-body">
        <h2>${esc(project.title)}</h2>
        ${project.author ? `<p class="by">${esc(project.author)}</p>` : ""}
        <ul class="tags">${markTags(project)}</ul>
      </div>
    </a>
  `).join("")}</div>`;
  document.title = "Catálogo · Programação com Arduinos";
}

function player(item, label) {
  if (item.kind === "youtube") {
    return `
      <figure class="stack">
        <div class="player">
          <iframe src="https://www.youtube-nocookie.com/embed/${esc(item.id)}" title="${esc(label)}" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen loading="lazy"></iframe>
        </div>
        <figcaption class="caption"><a href="${esc(item.url)}" target="_blank" rel="noopener">${esc(label)}</a></figcaption>
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
      return `<p><a href="${esc(item.url)}" target="_blank" rel="noopener">${esc(item.name)}</a> <span class="caption">${esc(size)} · o vídeo é grande e abre no GitHub</span></p>`;
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
  return `<li><a href="${esc(href)}" target="_blank" rel="noopener">${esc(item.name)}</a>${size ? ` <span class="caption">${esc(size)}</span>` : ""}</li>`;
}

function renderDetail(project) {
  const slides = project.presentations.filter((item) => item.kind === "slides");
  const pdfs = project.presentations.filter((item) => item.kind === "pdf");
  const links = project.presentations.filter((item) => item.kind === "link");
  const heroYoutube = project.presentations.find((item) => item.kind === "youtube");
  const media = project.presentations.filter((item) => {
    if (heroYoutube && item === heroYoutube) return false;
    return ["youtube", "drive", "video"].includes(item.kind);
  });
  let videoIndex = 0;
  const mediaHtml = media.map((item) => {
    videoIndex += 1;
    const label = media.length > 1 ? `Vídeo ${videoIndex}` : "Vídeo da apresentação";
    return player(item, label);
  }).join("");

  const textBlocks = [];
  if (project.summary) textBlocks.push(richText(project.summary));
  if (project.objectives) {
    textBlocks.push(`<h3 class="subhead">Objetivos</h3>${richText(project.objectives)}`);
  }
  if (project.resources) {
    textBlocks.push(`<h3 class="subhead">Recursos</h3>${richText(project.resources)}`);
  }
  const objectivesHtml = textBlocks.length
    ? `<div class="prose">${textBlocks.join("")}</div>`
    : `<p class="note">Os objetivos deste projeto ainda não estão descritos no repositório.</p>`;

  const docs = [...slides, ...pdfs, ...(project.documents || [])];
  const docsHtml = docs.length
    ? `<h3 class="subhead">Ficheiros da apresentação</h3><ul class="doc-list">${docs.map(fileLink).join("")}</ul>`
    : "";
  const linksHtml = links.length
    ? `<h3 class="subhead">Ligações</h3><ul class="link-list">${links.map((item) => `<li><a href="${esc(item.url)}" target="_blank" rel="noopener">${esc(item.name)}</a></li>`).join("")}</ul>`
    : "";
  const cover = coverUrl(project, true);
  const gallery = (project.images || []).filter((image) => image.cdnUrl !== cover).slice(0, 6);
  const galleryHtml = gallery.length
    ? `<h3 class="subhead">Imagens</h3><div class="gallery">${gallery.map((image) => `<a href="${esc(image.rawUrl)}" target="_blank" rel="noopener"><img src="${esc(image.cdnUrl)}" alt="${esc(image.name)}"></a>`).join("")}</div>`
    : "";
  const presentationBody = [
    !heroYoutube && !mediaHtml && !docsHtml && !linksHtml && !galleryHtml
      ? `<p class="note">Ainda não há apresentação associada a este projeto.</p>`
      : "",
    mediaHtml,
    docsHtml,
    linksHtml,
    galleryHtml,
  ].join("");

  const codeHtml = project.code.length
    ? `
      <div class="code-bar">
        <div class="code-files" role="tablist">
          ${project.code.map((file, index) => `<button type="button" class="file-btn" role="tab" data-file="${index}" aria-selected="${index === 0}">${esc(file.name)}</button>`).join("")}
        </div>
        <button type="button" class="text-btn" id="copy-code">Copiar</button>
      </div>
      <pre><code id="code-view" class="language-${esc(project.code[0].language || "cpp")}">A carregar o código…</code></pre>`
    : `<p class="note">Ainda não há código deste projeto no repositório. ${project.githubUrl ? `<a href="${esc(project.githubUrl)}" target="_blank" rel="noopener">Abrir a pasta</a>` : ""}</p>`;

  const hero = heroYoutube
    ? `<div class="hero player"><iframe src="https://www.youtube-nocookie.com/embed/${esc(heroYoutube.id)}" title="${esc(project.title)}" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen loading="lazy"></iframe></div>`
    : coverTag(project, "hero", true);

  view.innerHTML = `
    <article class="project">
      <a class="back" href="#/">← Voltar ao catálogo</a>
      ${hero}
      <header class="project-head">
        <p class="kicker">${esc(project.editionLabel)}</p>
        <h1>${esc(project.title)}</h1>
        <p class="meta">
          ${project.author ? `<span>${esc(project.author)}</span>` : ""}
          ${project.githubUrl ? `<a href="${esc(project.githubUrl)}" target="_blank" rel="noopener">Pasta no GitHub</a>` : ""}
        </p>
      </header>
      <section class="section">
        <h2>Designação e objetivos</h2>
        ${objectivesHtml}
      </section>
      ${presentationBody ? `<section class="section">
        <h2>Apresentação</h2>
        <div class="stack">${presentationBody}</div>
      </section>` : ""}
      <section class="section">
        <h2>Código</h2>
        ${codeHtml}
      </section>
    </article>`;

  document.title = `${project.title} · Programação com Arduinos`;
  if (project.code.length) loadCode(project, 0);
}

let loadedCode = "";

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
    node.textContent = loadedCode;
    node.className = `language-${file.language || "cpp"}`;
    if (window.hljs) window.hljs.highlightElement(node);
  } catch (error) {
    loadedCode = "";
    node.textContent = "Não foi possível mostrar o código nesta página. Abre o ficheiro no GitHub.";
  }
}

function render() {
  if (!catalog || !catalog.projects) {
    view.innerHTML = `<p class="empty">Não foi possível ler o catálogo.</p>`;
    return;
  }
  const id = currentId();
  const project = id ? findProject(id) : null;
  toolbar.hidden = Boolean(project);
  if (id && !project) {
    view.innerHTML = `<p class="empty">Este projeto não está no catálogo. <a href="#/">Voltar</a></p>`;
    return;
  }
  if (project) renderDetail(project);
  else renderList();
}

toolbar.addEventListener("click", (event) => {
  const edition = event.target.closest("[data-edition]");
  if (edition) {
    state.edition = edition.dataset.edition;
    renderToolbar();
    if (currentId()) location.hash = "#/";
    else renderList();
    return;
  }
  const flag = event.target.closest("[data-flag]");
  if (flag) {
    state[flag.dataset.flag] = !state[flag.dataset.flag];
    renderToolbar();
    if (currentId()) location.hash = "#/";
    else renderList();
  }
});

view.addEventListener("click", async (event) => {
  const fileButton = event.target.closest("[data-file]");
  const project = findProject(currentId());
  if (fileButton && project) {
    loadCode(project, Number(fileButton.dataset.file));
    return;
  }
  if (event.target.closest("#copy-code") && loadedCode) {
    try {
      await navigator.clipboard.writeText(loadedCode);
      event.target.textContent = "Copiado";
      setTimeout(() => { event.target.textContent = "Copiar"; }, 1200);
    } catch (error) {
      event.target.textContent = "Não copiou";
    }
  }
});

window.addEventListener("hashchange", () => {
  render();
  window.scrollTo(0, 0);
});

if (catalog) {
  const repo = document.getElementById("repo-link");
  const drive = document.getElementById("drive-link");
  if (catalog.repoUrl) repo.href = catalog.repoUrl;
  if (catalog.driveUrl) drive.href = catalog.driveUrl;
}

renderToolbar();
render();
