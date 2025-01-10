import asyncio
from aiocoap import *

class SensorResource(resource.Resource):
    """
    This resource simulates a sensor. When accessed, it provides
    a simple response like a temperature or sensor reading.
    """
    async def render_get(self, request):
        # Simulate a sensor reading
        sensor_data = "Temperature: 25.3°C"
        print(f"Received GET request, responding with: {sensor_data}")
        return Message(payload=sensor_data.encode('utf-8'))

# Set up the CoAP server
def main():
    root = resource.Site()
    root.add_resource(['sensor'], SensorResource())

    asyncio.Task(Context.create_server_context(root))
    print("CoAP Server is running. Listening on port 5683...")

    asyncio.get_event_loop().run_forever()

if __name__ == "__main__":
    main()
