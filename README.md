# nim_websi
**webview and socket and  interactif**<br />
<br /><br />
Nim bindings for zserge's Webview which is an excellent cross-platform single header webview library for C/C++ using Gtk  
<br />
thank-you [oskca](https://github.com/oskca/webview)  Your work inspired me a lot 
<br />  
require <br />
thank-you [https://github.com/treeform/ws](https://github.com/treeform/ws)  
<br /><br />  
**Linux**<br />

&rarr;&nbsp;JavaScriptCore/JavaScript.h<br />  
&rarr;&nbsp;gtk/gtk.h<br />  
&rarr;&nbsp;webkit2/webkit2.h<br />  
&rarr;&nbsp;X11/Xlib.h<br />  
&rarr;&nbsp;glib.h<br />  
&rarr;&nbsp;{.passL: staticExec"pkg-config --libs gtk+-3.0 webkit2gtk-4.0".}<br />  


&rarr;&nbsp;**update**&nbsp;&nbsp;:2021/04/21&nbsp;&nbsp;version WEBWSI_Hold.tar.gz&nbsp;&nbsp; in inject_css and set_font<br />  <br />  
&rarr;&nbsp;**update**&nbsp;&nbsp;:2021/04/21&nbsp;&nbsp;delete&nbsp; inject_css and set_font 
<br />  


** fonction:<br />
Allows you to process a web page as we practice with ex (Apache) but with the webview principle.
With a web-socket type interaction.

the possibility of having 3 distinctive screens (session):

&ndash;&nbsp;   the base page (new a window)

&ndash;&nbsp;   a modal page (new a window)

&ndash;&nbsp;   a dialog page (new a window)

Each of these pages works like a web page processed with a browser.   
  


But nothing is worth an example folder :"NimWeb"  
  
the folder for WScode / CODIUM is present in the NimWeb folder
you have an example of compilation with compil.sh
executable option-> no-pie 

download in .nimble folder "webwsi-0.1.0"   
  
image :  
  img : [EXEMPLE](https://github.com/AS400JPLPC/nim_webwsi/blob/master/exemple.png)

source:  
  src : [EXEMPLE](https://github.com/AS400JPLPC/nim_webwsi/blob/master/NimWeb/ecr00.nim)

**Procs**

proc init(w: webwsi): cint {...}

proc loop(w: webwsi; blocking: cint): cint {...}

proc eval(w: webwsi; js: cstring): cint {...}

proc setTitle(w: webwsi; title: cstring) {...}

proc setAltF4(w: webwsi; hint: bool) {...}

proc showInit(w: webwsi) {...}

proc setDestroy(w: webwsi) {...}

proc setDeletable(w: webwsi; hint: bool) {...}

proc setTaskBar(w: webwsi; hint: bool) {...}

proc setResizable(w: webwsi; hint: bool) {...}

proc setFocus(w: webwsi) {...}

proc setFullscreen(w: webwsi) {...}

proc setColor(w: webwsi; r, g, b, a: uint8) {...}

proc terminate(w: webwsi) {...}

proc exit(w: webwsi) {...}

proc debug(format: cstring) {...}

proc printLog(s: cstring) {...}

proc fullscreen(w: webwsi) {...}

proc chgUrl(w: webwsi; url: cstring) {...}

proc setMessage(w: webwsi; flags: cint; title: cstring; arg: cstring) {...}

proc showModal(w: webwsi; url: cstring) {...}

proc hideModal(w: webwsi) {...}

proc showDialog(w: webwsi; url: cstring) {...}

proc hideDialog(w: webwsi) {...}

proc showWindow(w: webwsi; r: webwsi) {...}

proc externalInvokeCB=(w: webwsi; cb: ExternalInvokeCb) {...}

    Set external invoke callback for webwsi 

proc newwebwsi(title = "webwsi"; url = ""; width = 0; height = 0;
               resizable = true; debug = false; cb: ExternalInvokeCb = nil): webwsi {...}

    newwebwsi creates and opens a new webwsi window using the given settings. This function will do webwsi init 

proc dispatch(w: webwsi; fn: DispatchFn) {...}

proc info(w: webwsi; msg: string) {...}

    Show one alert box 

proc warn(w: webwsi; msg: string) {...}

proc erreur(w: webwsi; msg: string) {...}

proc query(w: webwsi; msg: string) {...}

proc msg(w: webwsi; title, msg: string) {...}

    Show one message box 

proc run(w: webwsi) {...}

    run starts the main UI loop until the user closes the webwsi window or Terminate() is called. 

proc bindProc[P, R](w: webwsi; scope, name: string; p: (proc (param: P): R))

proc bindProcNoArg(w: webwsi; scope, name: string; p: proc ()) {...}

    ugly hack or macro will fail 

proc bindProc[P](w: webwsi; scope, name: string; p: proc (arg: P))

proc endClose(w: var webwsi) {...}




**Macros**

macro bindProcs(w: webwsi; scope: string; n: untyped): untyped

    bind procs like:

    proc fn[T, U](arg: T): U
    proc fn[T](arg: T)
    proc fn()

    to webwsi w, in scope scope then you can invode in js side, like this:

    scope.fn(arg)



Made with Nim. Generated: 2021-04-21 10:11:14 UTC
