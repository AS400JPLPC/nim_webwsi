{.passC: "-DWEBWSI_STATIC -DWEBWSI_IMPLEMENTATION".}
{.passC: "-I" & currentSourcePath().substr(0, high(currentSourcePath()) - 4) .}
when defined(linux):
  {.passC: "-DWEBWSI_GTK=1 " &
          staticExec"pkg-config --cflags gtk+-3.0 webkit2gtk-4.0".}
  {.passL: staticExec"pkg-config --libs gtk+-3.0 webkit2gtk-4.0".}


import tables, strutils , uri
import macros
import json

##
## Hight level api and javascript bindings to easy bidirectonal
## message passsing for ``nim`` and the ``webwsi`` .
##

type
  ExternalInvokeCb* = proc (w: webwsi; arg: string)
  WebwsiPrivObj{.importc: "struct webwsi_priv", header: "webwsi.h", bycopy.} = object
  WebwsiObj* {.importc: "struct webwsi", header: "webwsi.h", bycopy.} = object
    url* {.importc: "url".}: cstring
    title* {.importc: "title".}: cstring
    width* {.importc: "width".}: cint
    height* {.importc: "height".}: cint
    resizable* {.importc: "resizable".}: cint
    fullscreen* {.importc: "fullscreen".}: cint
    debug* {.importc: "debug".}: cint
    toucheAltf4* {.importc: "resizable".}: cint
    buttonClose* {.importc: "resizable".}: cint
    taskbar* {.importc: "resizable".}: cint
    repDialog* {.importc: "repDialog".}: cint

    mtitle* {.importc: "mtitle".}: cint
    murl* {.importc: "murl".}: cint
    mwidth* {.importc: "mwidh".}: cint
    mheight* {.importc: "mheight".}: cint
    mfullscreen* {.importc: "mdlfullscreen".}: cint

    invokeCb {.importc: "external_invoke_cb".}: pointer
    priv {.importc: "priv".}: WebwsiPrivObj
    modal {.importc: "priv".}: WebwsiPrivObj
    userdata {.importc: "userdata".}: pointer
  webwsi* = ptr WebwsiObj

  DialogType* {.size: sizeof(cint).} = enum
    dtOpen = 0, dtSave = 1, dtAlert = 2

const
  dFlagFile* = 0
  dFlagDir* = 1

const
  cFlagNone :cint = -4
  cFlagInfo :cint = 0
  cFlagWarn :cint = 1
  cFlagError :cint  = 2
  cFlagQuery :cint  = 3

type
  DispatchFn* = proc()


proc init*(w: webwsi): cint {.importc: "webwsi_init", header: "webwsi.h".}

proc loop*(w: webwsi; blocking: cint): cint {.importc: "webwsi_loop", header: "webwsi.h".}

proc eval*(w: webwsi; js: cstring): cint {.importc: "webwsi_eval", header: "webwsi.h".}

proc injectCss*(w: webwsi; css: cstring): cint {.importc: "webwsi_inject_css", header: "webwsi.h".}

proc setTitle*(w: webwsi; title: cstring) {.importc: "webwsi_set_title", header: "webwsi.h".}

proc setAltF4*(w: webwsi;  hint: bool)  {.importc: "webwsi_set_altf4", header: "webwsi.h".}

proc showInit*(w: webwsi;)  {.importc: "webwsi_show_init", header: "webwsi.h".}


proc setDestroy*(w: webwsi;)  {.importc: "webwsi_destroy", header: "webwsi.h".}

proc setDeletable*(w: webwsi;  hint: bool)  {.importc: "webwsi_setDeletable", header: "webwsi.h".}

proc setTaskBar*(w: webwsi; hint: bool) {.importc: "webwsi_setTaskBar", header: "webwsi.h".}


proc setResizable*(w: webwsi;  hint: bool) {.importc: "webwsi_setresizable", header: "webwsi.h".}

proc setFocus*(w: webwsi) {.inline.} ={.emit: "gtk_widget_grab_focus(GTK_WINDOW(`w`->priv.window));".}

proc setFont*(w: webwsi; hint: cstring) {.importc: "webwsi_set_font", header: "webwsi.h".}

proc setFullscreen*(w: webwsi;) {.importc: "webwsi_set_fullscreen", header: "webwsi.h".}

proc setColor*(w: webwsi; r,g,b,a: uint8) {.importc: "webwsi_set_color", header: "webwsi.h".}


proc dispatch(w: webwsi; fn: pointer; arg: pointer) {.importc: "webwsi_dispatch", header: "webwsi.h".}
proc terminate*(w: webwsi) {.importc: "webwsi_terminate", header: "webwsi.h".}
proc exit*(w: webwsi) {.importc: "webwsi_exit", header: "webwsi.h".}
proc debug*(format: cstring) {.varargs, importc: "webwsi_debug", header: "webwsi.h".}
proc printLog*(s: cstring) {.importc: "webwsi_print_log", header: "webwsi.h".}

proc dialogmsg(w: webwsi; flags: cint; title: cstring; arg: cstring;): cint {.importc: "webwsi_dialog_msg", header:"webwsi.h".}


proc fullscreen*(w: webwsi;) {.importc: "webwsi_set_fullscreen", header: "webwsi.h".}
proc chgUrl*(w: webwsi; url:cstring) {.importc: "webwsi_change_url", header: "webwsi.h".}
proc setMessage*(w: webwsi;flags:cint, title : cstring, arg :cstring) {.importc:"webwsi_setMessage", header: "webwsi.h".}
proc showModal*(w: webwsi;  url : cstring;)  {.importc: "webwsi_modal", header: "webwsi.h".}
proc hideModal*(w: webwsi;)  {.importc: "webwsi_hide_modal", header: "webwsi.h".}
proc showDialog*(w: webwsi;  url : cstring;)  {.importc: "webwsi_dialogx", header: "webwsi.h".}
proc hideDialog*(w: webwsi;)  {.importc: "webwsi_hide_dialogx", header: "webwsi.h".}
proc showWindow*(w: webwsi;r: webwsi;)  {.importc: "webwsi_show", header: "webwsi.h".}

type
  # ExternalProc[P, R] = proc(param: P, ret: var R): int
  CallHook = proc(params: string): string # json -> proc -> json
  MethodInfo* = object
    scope*: string
    name*: string
    args*: string # json string

# for bindProc
var eps = newTable[webwsi, TableRef[string, TableRef[string, CallHook]]]()

# easy callbacks
var cbs = newTable[webwsi, ExternalInvokeCb]()

proc generalExternalInvokeCallback(w: webwsi, arg: cstring) {.exportc.} =
  var handled = false
  if eps.hasKey(w):
    try:
      var mi = parseJson($arg).to(MethodInfo)
      if hasKey(eps[w], mi.scope) and hasKey(eps[w][mi.scope], mi.name):
        discard eps[w][mi.scope][mi.name](mi.args) # TODO handle return values using js callbacks
        handled = true
    except:
      echo getCurrentExceptionMsg()
  elif cbs.hasKey(w):
    cbs[w](w, $arg)
    handled = true
  if handled == false:
    echo "external invode:'", arg, "' not handled"

proc `externalInvokeCB=`*(w: webwsi, cb: ExternalInvokeCb)=
  ## Set external invoke callback for webwsi
  cbs[w] = cb

proc newwebwsi*(title="webwsi", url="",
                width=0, height=0,
                resizable=true, debug=false,
                cb:ExternalInvokeCb=nil): webwsi =
  ## ``newwebwsi`` creates and opens a new webwsi window using the given settings.
  ## This function will do webwsi ``init``
  var w = cast[webwsi](alloc0(sizeof(WebwsiObj)))
  w.title = title
  w.url = url
  w.width = width.cint
  w.height = height.cint
  w.resizable = if resizable: 1 else: 0
  w.debug = if debug: 1 else: 0
  w.invokeCb = generalExternalInvokeCallback
  if cb != nil:
    w.externalInvokeCB=cb
  if w.init() != 0: return nil
  return w


# for dispatch

var dispatchTable = newTable[int, DispatchFn]()

proc generalDispatchProc(w: webwsi, arg: pointer) {.exportc.} =
  let idx = cast[int](arg)
  let fn = dispatchTable[idx]
  fn()

proc dispatch*(w: webwsi, fn: DispatchFn) =
  let idx = dispatchTable.len()+1
  dispatchTable[idx] = fn
  dispatch(w, generalDispatchProc, cast[pointer](idx))


proc info*(w: webwsi; msg: string) =
  ## Show one alert box
  var repDialog :cint = 999
  while repDialog == 999 :
    repDialog = dialogmsg(w,cFlagInfo,"Info", msg)
    if repDialog == cFlagNone : repDialog = 999
  w.repDialog = repDialog

proc warn*(w: webwsi; msg: string) =
  var repDialog :cint = 999
  while repDialog == 999 :
    repDialog = dialogmsg(w,cFlagWarn,"Warning", msg)
    if repDialog == cFlagNone : repDialog = 999
  w.repDialog = repDialog

# not use error mismatch conflicting  prefer erreur in french  = error
# Title default "Error" or recieved msg "title|my-message"
proc erreur*(w: webwsi; msg: string) =
  var repDialog :cint = 999
  var title: string = "Error"
  var msgx: string = msg
  if count(msgx , '|') > 0 :
    var rmsg = msgx.split("|")
    title = "tot"
    msgx = rmsg[1]

  while repDialog == 999 :
    repDialog = dialogmsg(w,cFlagError,title, msgx)
    if repDialog == cFlagNone : repDialog = 999
  w.repDialog = repDialog
  w.terminate()

proc query*(w: webwsi; msg: string) =
  var repDialog :cint = 999
  while repDialog == 999 :
    repDialog = dialogmsg(w,cFlagQuery,"Query", msg)
    if repDialog == cFlagNone : repDialog = 999
  w.repDialog = repDialog

proc msg*(w: webwsi, title, msg: string) =
  ## Show one message box
  discard dialogmsg(w,cFlagInfo,"Message", msg)


proc run*(w: webwsi)=
  ## ``run`` starts the main UI loop until the user closes the webwsi window or
  ## Terminate() is called.
  while w.loop(1) == 0:
    discard


const
  jsTemplate = """
if (typeof $2 === 'undefined') {
	$2 = {};
}
$2.$1 = function(arg) {
	window.external.invoke(
    JSON.stringify(
      {scope: "$2", name: "$1", args: JSON.stringify(arg)}
    )
  );
};
"""
  jsTemplateOnlyArg = """
if (typeof $2 === 'undefined') {
	$2 = {};
}
$2.$1 = function(arg) {
	window.external.invoke(
    JSON.stringify(
      {scope: "$2", name: "$1", args: JSON.stringify(arg)}
    )
  );
};
"""
  jsTemplateNoArg = """
if (typeof $2 === 'undefined') {
	$2 = {};
}
$2.$1 = function() {
	window.external.invoke(
    JSON.stringify(
      {scope: "$2", name: "$1", args: ""}
    )
  );
};
"""

proc bindProc*[P, R](w: webwsi, scope, name: string, p: (proc(param: P): R)) =
  proc hook(hookParam: string): string =
    var
      paramVal: P
      retVal: R
    try:
      let jnode = parseJson(hookParam)
      echo jnode
      paramVal = jnode.to(P)
    except:
      return "parse args failed: " & getCurrentExceptionMsg()
    retVal = p(paramVal)
    return $(%*retVal) # ==> json
  discard eps.hasKeyOrPut(w, newTable[string, TableRef[string, CallHook]]())
  discard hasKeyOrPut(eps[w], scope, newTable[string, CallHook]())
  eps[w][scope][name] = hook
  # TODO eval jscode
  w.dispatch(proc() = discard w.eval(jsTemplate%[name, scope]))

proc bindProcNoArg*(w: webwsi, scope, name: string, p: proc()) =
  ## ugly hack or macro will fail
  proc hook(hookParam: string): string =
    p()
    return ""
  discard eps.hasKeyOrPut(w, newTable[string, TableRef[string, CallHook]]())
  discard hasKeyOrPut(eps[w], scope, newTable[string, CallHook]())
  eps[w][scope][name] = hook
  # TODO eval jscode
  w.dispatch(proc() = discard w.eval(jsTemplateNoArg%[name, scope]))

proc bindProc*[P](w: webwsi, scope, name: string, p: proc(arg:P)) =
  proc hook(hookParam: string): string =
    var
      paramVal: P
    try:
      let jnode = parseJson(hookParam)
      paramVal = jnode.to(P)
    except:
      return "parse args failed: " & getCurrentExceptionMsg()
    p(paramVal)
    return ""
  discard eps.hasKeyOrPut(w, newTable[string, TableRef[string, CallHook]]())
  discard hasKeyOrPut(eps[w], scope, newTable[string, CallHook]())
  eps[w][scope][name] = hook
  # TODO eval jscode
  w.dispatch(proc() = discard w.eval(jsTemplateOnlyArg%[name, scope]))

macro bindProcs*(w: webwsi, scope: string, n: untyped): untyped =
  ## bind procs like:
  ##
  ## .. code-block:: nim
  ##
  ##    proc fn[T, U](arg: T): U
  ##    proc fn[T](arg: T)
  ##    proc fn()
  ##
  ## to webwsi ``w``, in scope ``scope``
  ## then you can invode in js side, like this:
  ##
  ## .. code-block:: js
  ##
  ##    scope.fn(arg)
  ##
  expectKind(n, nnkStmtList)
  let body = n
  for def in n:
    expectKind(def, nnkProcDef)
    let params = def.params()
    let fname = $def[0]
    # expectKind(params[0], nnkSym)
    if params.len() == 1 and params[0].kind() == nnkEmpty: # no args
      body.add(newCall("bindProcNoArg", w, scope, newLit(fname), newIdentNode(fname)))
      continue
    if params.len() > 2 :
      error("""only proc like `proc fn[T, U](arg: T): U` or
              `proc fn[T](arg: T)` or
              `proc()`
            is allowed""",
            def)
    body.add(newCall("bindProc", w, scope, newLit(fname), newIdentNode(fname)))
  result = newBlockStmt(body)
  echo repr result

proc endClose*(w: var  webwsi;) =
  w.url = "";
  w.terminate()

