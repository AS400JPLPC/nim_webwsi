import ws, asyncdispatch, asynchttpserver
import asyncfile
import os
import threadpool
import json
import strformat
import webwsi


{.warning[CStringConv]: off.}


proc observer(w:webwsi;m:webwsi;d:webwsi;) {.thread}   =
  echo "start observer"

  var server = newAsyncHttpServer()

  proc cb(req: Request ; ){.async.} =

    #----------------------------------------------------
    #
    #----------------------------------------------------
    type nameWork = object
      page: string
      fonc: string
      arg:  string

    type recordValidate = object
      nom: string
      prenom: string
      date:string
      nrue:string
      rue1:string
      rue2:string
      ville:string


    #----------------------------------------------------
    # module multi windows
    #----------------------------------------------------
    const
      cFlagInfo :cint = 0
      cFlagWarn :cint = 1
      cFlagError :cint  = 2
      cFlagQuery :cint  = 3
      RESPONSE_REJECT       = -2
#      RESPONSE_OK           = -5
      RESPONSE_YES          = -8
      RESPONSE_NO           = -9

    #----------------------------------------------------
    echo "New websocket customer arrived!"
    try:
      var ws = await newWebSocket(req,"testwebviewtoserver")
      while true:
        let packet = await ws.receiveStrPacket()
        let jsonObject = parseJson(packet)
        let element = to(jsonObject, nameWork)

        echo "element length: ", element.arg.len

        #let nameFonc   = jsonObject["name"].getStr()
        echo element.page,"--",element.fonc

        case element.page
          of "ecr00":
            # execute fonction
            case element.fonc:

              of "close" :
                if element.arg == "close" :
                  terminate(w)


              of "screen"  :
                if element.arg == "fullScreen" : fullscreen (w)


              of "url"   :
                if element.arg > " " :
                  var html:string = $fmt"file:{getCurrentDir()}/{element.arg}.html"
                  echo html
                  chgUrl(w,html)


              of "info":
                if element.arg > "" :
                  w.setMessage(cFlagInfo,"Info", "xxx")
                  while w.repDialog == 999:
                    sleep(1)
                  echo "serveur -->w.repDialog:", w.repDialog
                  #let jsonInfo = %* {"fonc": element.fonc, "arg": element.arg}
                  #await ws.send( $jsonInfo)
                  #await ws.send($element.name)
                  #await ws.send($element.arg)

              of "warn" :
                  if element.arg > "" :
                    w.setMessage(cFlagWarn,"Warning", "des erreurs de saisie sont détestés\nveuillez corriger")
                    while w.repDialog == 999:
                      sleep(1)


              of "error" :
                  if element.arg > "" :
                    w.setMessage(cFlagError,"Error",element.arg)
                    while w.repDialog == 999:
                      sleep(1)

              of "question" :
                if element.arg > "" :
                  while true :
                    w.setMessage(cFlagQuery,"Question","Peut-tu venir boire une bière bien fraîche")
                    while w.repDialog == 999 :
                      sleep(1)
                    echo "serveur YES-NO -->w.repDialog:", w.repDialog
                    if w.repDialog != RESPONSE_REJECT : break

                  case w.repDialog :
                    of RESPONSE_YES :
                        let jsonMsg = %* {"fonc": "QueryMsg", "arg": "ok j'arrive"}
                        await ws.send( $jsonMsg)
                    of RESPONSE_NO :
                        let jsonMsg = %* {"fonc": "QueryMsg", "arg": "ok je te téléphone\nje suis en conférence"}
                        await ws.send( $jsonMsg)
                    else : discard

              of "getjson" :
                if element.arg > " " :
                  var fname:string = $fmt"{getCurrentDir()}/code/{element.arg}.json"
                  echo fname
                  var file = openAsync(fname)
                  var datajson  = await file.readAll()
                  echo "file"
                  let jsonGet = %* {"fonc": element.arg, "arg": datajson }
                  await ws.send($jsonGet)
                  echo "end-data"
                  file.close()

              of "record":
                  if element.arg > "" :

                    echo "element.arg \n", element.arg
                    let jsonRecord = parseJson(element.arg)
                    let record = to(jsonRecord, recordValidate)
                    echo "record objet \n", $record
                    echo "record json \n",%record
                    echo "imagine reveide sql return json pour record html "
                    let jsonRcd = %* {"fonc":  element.fonc, "arg": $(%record)}
                    await ws.send( $jsonRcd)

              of "modal" :
                  if element.arg > "" :
                    echo "fenetre modal "
                  #w.hideWindow()
                    var html:string = $fmt"file:{getCurrentDir()}/{element.arg}.html"
                    echo html
                    m.showModal(html)
                    echo "fenetre modal "

              of "dialog" :
                  if element.arg > "" :
                    echo "fenetre dialog "
                  #w.hideWindow()
                    var html:string = $fmt"file:{getCurrentDir()}/{element.arg}.html"
                    echo html;
                    d.showDialog(html)

              else: echo "non reconnue",element.page,"---", element.fonc

          of "ecr01":
            # execute fonction
            case element.fonc:
              of "screen"  :
                  if element.arg == "fullScreen" : fullscreen (w)

              of "url"   :
                if element.arg > " " :
                  var html:string = $fmt"file:{getCurrentDir()}/{element.arg}.html"
                  echo html
                  chgUrl(w,html)

              else: echo "non reconnue",element.page,"---", element.fonc

          of "ecr00A":
            # execute fonction
            case element.fonc:
              of "screen"  :
                  if element.arg == "fullScreen" : fullscreen (m)

              of "endPage" :
                  if element.arg > "" :
                    echo "fermer modal "

                    m.hideModal()
                    w.showWindow(m)
              of "info":
                if element.arg > "" :
                  m.setMessage(cFlagInfo,"Info", "xxx")
                  while m.repDialog == 999:
                    sleep(1)
                  echo "serveur -->w.repDialog:", m.repDialog
                  #let jsonInfo = %* {"fonc": element.fonc, "arg": element.arg}
                  #await ws.send( $jsonInfo)
                  #await ws.send($element.name)
                  #await ws.send($element.arg)

              of "warn" :
                  if element.arg > "" :
                    m.setMessage(cFlagWarn,"Warning", "des erreurs de saisie sont détestés\nveuillez corriger")
                    while m.repDialog == 999:
                      sleep(1)


              of "error" :
                  if element.arg > "" :
                    m.setMessage(cFlagError,"Error",element.arg)
                    while m.repDialog == 999:
                      sleep(1)

              of "question" :
                if element.arg > "" :
                  while true :
                    m.setMessage(cFlagQuery,"Question","Peut-tu venir boire une bière bien fraîche")
                    while m.repDialog == 999 :
                      sleep(1)
                    echo "serveur YES-NO -->w.repDialog:", m.repDialog
                    if m.repDialog != RESPONSE_REJECT : break

                  case m.repDialog :
                    of RESPONSE_YES :
                        let jsonMsg = %* {"fonc": "QueryMsg", "arg": "ok j'arrive"}
                        await ws.send( $jsonMsg)
                    of RESPONSE_NO :
                        let jsonMsg = %* {"fonc": "QueryMsg", "arg": "ok je te téléphone\nje suis en conférence"}
                        await ws.send( $jsonMsg)
                    else : discard

              of "getjson" :
                if element.arg > " " :
                  var fname:string = $fmt"{getCurrentDir()}/code/{element.arg}.json"
                  echo fname
                  var file = openAsync(fname)
                  var datajson  = await file.readAll()
                  echo "file"
                  let jsonGet = %* {"fonc": element.arg, "arg": datajson }
                  await ws.send($jsonGet)
                  echo "end-data"
                  file.close()

              else: echo "non reconnue",element.page,"---", element.fonc

          of "ecr02":
            # execute fonction
            case element.fonc:
              of "screen"  :
                  if element.arg == "fullScreen" : fullscreen (d)

              of "endPage" :
                  if element.arg > "" :
                    echo "fermer dialog "
                    d.hideDialog()
                    w.showWindow(d)

              of "warn" :
                  if element.arg > "" :
                    d.setMessage(cFlagWarn,"Warning", "des erreurs de saisie sont détestés\nveuillez corriger")
                    while d.repDialog == 999:
                      sleep(1)


              of "error" :
                  if element.arg > "" :
                    d.setMessage(cFlagError,"Error",element.arg)
                    while d.repDialog == 999:
                      sleep(1)

              of "getjson" :
                if element.arg > " " :
                  var fname:string = $fmt"{getCurrentDir()}/code/{element.arg}.json"
                  echo fname
                  var file = openAsync(fname)
                  var datajson  = await file.readAll()
                  echo "file"
                  let jsonGet = %* {"fonc": element.arg, "arg": datajson }
                  await ws.send($jsonGet)
                  echo "end-data"
                  file.close()

              of "modal" :
                  if element.arg > "" :
                    echo "fenetre modal "
                    #w.hideWindow()
                    var html:string = $fmt"file:{getCurrentDir()}/{element.arg}.html"
                    echo html
                    m.showModal(html)
              else: echo "non reconnue",element.page,"---", element.fonc




          else: echo "non reconnue",element.page
    except WebSocketError:
      echo "socket closed:", getCurrentExceptionMsg()

  waitFor server.serve(Port(9200),cb)





#[

  a) page html initial
  b) initialisation de paramètre
        setDeletable -> affiche le button delete
        setAltF4 -> accepte alt-f4
        setTaskBar -> suppression de la barre l'onglet
        setResizable -> autorise resize
  c) spawnX -> le seveur sous threadpool dans un coeur different allège le system

  d) observer -> le coeur du programme
      a) thread
      b) conserve l'initialisation des parametres s'execute dans le même run()
      c) les commandes bindProcs sont reporté dans l'observer
      d) ne traite que les data
      f) délagation de la partie design , cotrôle --> HTML5-Javascript

  e) run() exxécute l'aplication

]#


# page html initial

var w = newwebwsi("CONTACT", $fmt"file:{getCurrentDir()}/ecr00.html")
var m = newwebwsi("CONTACT-MODAL","")
var d = newwebwsi("CONTACT-DIALOG","")

w.setDeletable(true)
w.setAltF4(true)
w.setTaskBar(false)
w.setResizable(true)
w.showInit()


m.setDeletable(false)
m.setAltF4(true)
m.setTaskBar(true)
m.setResizable(true)

d.setDeletable(false)
d.setAltF4(true)
d.setTaskBar(true)
d.setResizable(false)


spawnX(observer(w,m,d))

w.run()

w.exit()