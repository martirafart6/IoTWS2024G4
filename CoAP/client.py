import asyncio
import random
from aiocoap import *

async def update_sensor_data():
    # Create a CoAP client context
    protocol = await Context.create_client_context()

    try:
        while True:
            # Generate random sensor data
            random_temperature = round(random.uniform(20.0, 30.0), 2)
            payload = f"Sensor reading: {random_temperature}°C".encode('utf-8')
            
            # Log the payload being sent
            print(f"Sending PUT payload: {payload.decode('utf-8')}")
            
            # Create a PUT request message
            request = Message(code=PUT, uri="coap://localhost/sensor", payload=payload)
            
            # Send the PUT request and wait for the response
            response = await protocol.request(request).response
            print(f"Result: {response.code}\n{response.payload.decode('utf-8')}\n")
            
            # Wait 5 seconds before sending the next request
            await asyncio.sleep(5)

    except KeyboardInterrupt:
        print("Client stopped by the user.")
    finally:
        await protocol.shutdown()

if __name__ == "__main__":
    asyncio.run(update_sensor_data())
