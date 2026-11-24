@echo off
cd /d "%~dp0"
echo Catalogo em http://localhost:8080
python -m http.server 8080
