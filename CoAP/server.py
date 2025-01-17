import datetime
import logging

import asyncio

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
        self.content = b"Initial sensor data"

    async def render_put(self, request):
        logging.info(f"Client connected at {datetime.datetime.now()} via PUT")
        print(f"Received PUT payload: {request.payload.decode('utf-8')}")
        self.content=request.payload
        return aiocoap.Message(code=aiocoap.CHANGED, payload=b"Sensor data updated successfully")
    


# logging setup

logging.basicConfig(level=logging.INFO)
#logging.getLogger("coap-server").setLevel(logging.DEBUG)


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
