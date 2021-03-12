import websocket, asynchttpserver, asyncnet, asyncdispatch
import threadpool
import json
import strformat
import os
import webviews


proc observer(w:Webview;) {.thread} =

  echo "start observer"

  let server = newAsyncHttpServer()

  proc cb(req: Request) {.async.} =
    let (ws, error) = await verifyWebsocketRequest(req, "myfancyprotocol")

    if ws.isNil:
      echo "WS negotiation failed: ", error
      await req.respond(Http400, "Websocket negotiation failed: " & $error)
      req.client.close()
      return

    echo "New websocket customer arrived!"
    while true:
      let (opcode, data) = await ws.readData()
      try:
        echo "(opcode: ", opcode, ", data length: ", data.len, ")"

        case opcode
        of Opcode.Text:
          await ws.sendText("thanks for the data!")
        of Opcode.Binary:
          await ws.sendBinary(data)
        of Opcode.Close:
          asyncCheck ws.close()
          let (closeCode, reason) = extractCloseData(data)
          echo "socket went away, close code: ", closeCode, ", reason: ", reason
        else: discard
      except:
        echo "encountered exception: ", getCurrentExceptionMsg()

  waitFor server.serve(Port(9200), cb)

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
      f) delagation de la partie design , cotrôle --> HTML5-Javascript

  e) run() exxécute l'aplication

]#


# page html initial

var w = newWebView("CONTACT", "file:/home/soleil/NimWeb/ecr00.html")

w.setDeletable(true)
w.setAltF4(true)
w.setTaskBar(false)
w.setResizable(true)

spawnX(observer(w))

w.run()

w.exit()
