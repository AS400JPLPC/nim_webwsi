# Package

version       = "0.1.0"
author        = "webview socket interactif AS400JPLPC"
description   = "Auteur de base  https://github.com/oskca/webview"
license       = "(MIT or Apache License 2.0) and Simplified BSD"

# Dependencies

requires "nim >= 1.2"


task docs, "generate doc":
    exec "nim doc2 -o:docs/websi.html webwsi.nim"


