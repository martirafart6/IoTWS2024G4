#import datetime
import logging

import asyncio
import random

import aiocoap.resource as resource
from aiocoap.numbers.contentformat import ContentFormat
import aiocoap


class SensorResource(resource.Resource):
    """
    Example resource which supports the GET and PUT methods. It sends large
    responses, which trigger blockwise transfer.
    """
    def __init__(self):
        super().__init__()
        self.generate_random_data()

    def generate_random_data(self):
        self.content = f"Random sensor reading: {random.uniform(20.0, 30.0):.2f}°C".encode('utf-8')

    async def render_get(self, request):
        self.generate_random_data()  # Generar un nuevo dato cada vez que se llama a GET
        return aiocoap.Message(payload=self.content)

    #async def render_put(self, request):
        #print("PUT payload: %s" % request.payload)
        #return aiocoap.Message(code=aiocoap.CHANGED, payload=b"PUT method not supported for this resource")



# logging setup

logging.basicConfig(level=logging.INFO)
logging.getLogger("coap-server").setLevel(logging.DEBUG)


async def main():
    # Resource tree creation
    root = resource.Site()

    root.add_resource(
        [".well-known", "core"], resource.WKCResource(root.get_resources_as_linkheader)
    )
    root.add_resource(['sensor'], SensorResource())

    await aiocoap.Context.create_server_context(root)

    # Run forever
    await asyncio.get_running_loop().create_future()


if __name__ == "__main__":
    asyncio.run(main())
