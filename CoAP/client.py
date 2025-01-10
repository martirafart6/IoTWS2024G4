import asyncio
from aiocoap import *

async def get_sensor_data():
    # Context of 
    protocol = await Context.create_client_context()

    try:
        while True:
            # New application
            request = Message(code=GET, uri="coap://localhost/sensor")
            
            # GET application
            response = await protocol.request(request).response
            print(f"Result: {response.code}\n{response.payload.decode('utf-8')}\n")
            
            # Wait 5 sec for the next application
            await asyncio.sleep(5)  

    except KeyboardInterrupt:
        print("Loop detenido por el usuario.")
    finally:
        await protocol.shutdown()

if __name__ == "__main__":
    asyncio.run(get_sensor_data())
