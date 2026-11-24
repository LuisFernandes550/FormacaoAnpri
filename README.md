# Catálogo da formação Programação com Arduinos

O site dos projetos está em [https://luisfernandes550.github.io/](https://luisfernandes550.github.io/).

Página para consultar os projetos dos formandos: designação e objetivos, apresentação (vídeo, diapositivos ou PDF) e código.

Os projetos vêm do repositório [FormacaoAnpri](https://github.com/LuisFernandes550/FormacaoAnpri). Os vídeos e ligações mais antigos vêm da [pasta no Google Drive](https://drive.google.com/drive/folders/1S66_71QPppwhJrDpkIF-GApYYr0ZmdBV).

## Ver neste computador

Na pasta do projeto, executa `servir.bat` ou:

```
python -m http.server 8080
```

Abre [http://localhost:8080](http://localhost:8080).

O servidor local é necessário para o código aparecer dentro da página. Os vídeos do YouTube e as ligações para o GitHub também funcionam ao abrir o `index.html` diretamente.

## Atualizar o catálogo

Quando houver projetos novos no GitHub:

```
python scripts/build_catalog.py
```

Os objetivos saem do README de cada projeto e, quando existe, da proposta em Word (secções Designação, Enquadramento e Objetivos).

Para preencher objetivos à mão, edita `docs/data/objetivos.json`. A chave é a pasta do projeto:

```json
{
  "2022_T1/Candeeiro": "Acender um candeeiro conforme a luz da sala."
}
```

Depois volta a correr o script acima.

## No GitHub

O site público está em [https://luisfernandes550.github.io/FormacaoAnpri/](https://luisfernandes550.github.io/FormacaoAnpri/). Os ficheiros da página ficam na pasta `docs`. O repositório [FormacaoAnpri](https://github.com/LuisFernandes550/FormacaoAnpri) continua a guardar os projetos dos formandos.
