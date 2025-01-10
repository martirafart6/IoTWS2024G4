import asyncio
from aiocoap import *

async def main():
    # CoAP server URL
    uri = "coap://localhost/sensor"

    protocol = await Context.create_client_context()

    # Create and send a GET request
    request = Message(code=GET, uri=uri)

    try:
        response = await protocol.request(request).response
        print(f"Response from server: {response.payload.decode('utf-8')}")
    except Exception as e:
        print(f"Failed to fetch data from server: {e}")

if __name__ == "__main__":
    asyncio.run(main())
