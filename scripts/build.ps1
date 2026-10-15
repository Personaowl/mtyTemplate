[CmdletBinding()]
param()
$ErrorActionPreference = 'Stop'
if (-not (Get-Command latexmk -ErrorAction SilentlyContinue)) {
    throw 'latexmk was not found. Install TeX Live with XeLaTeX and add its bin directory to PATH.'
}
$projectRoot = Split-Path -Parent $PSScriptRoot
Push-Location -LiteralPath $projectRoot
try {
    & latexmk main.tex
    if ($LASTEXITCODE -ne 0) { throw 'LaTeX build failed: main.tex' }
    Write-Host 'PDF: build/main.pdf'
} finally {
    Pop-Location
}
