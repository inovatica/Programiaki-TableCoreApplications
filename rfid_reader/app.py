import websockets, json, asyncio
from reader import reader

addresses = {0x34, 0x30, 0x35, 0x31, 0x36, 0x32}

count = {}
cards = {}

for addr in addresses:
    r = Reader(addr)
    count[addr] = r.getCount()
    cards[addr] = []

    @asyncio.coroutine
    def sendToWs(cardNumber,address):
        websocket = yield from websockets.connect('ws://localhost:8886/')

        r = {}
        r["cards"] = cards
        merge = str(address) + '_' + str(cardNumber)
        r["change"] = {merge: cards[address][cardNumber]}

        print("sent to WS")
        print(r)

        try:
            yield from websocket.send(json.dumps(r, ensure_ascii=False))

        finally:
            yield from websocket.close()

            writeBus(addr,72)
            
            while True:
                for addr in addresses:
                    r = Reader(addr)
                    status = r.proceed()
                    if(status != False):
                        cards[addr][status["cardNumber"]] = status["tagId"]
            #asyncio.get_event_loop().run_until_complete(sendToWs(cardNumber,addr))
