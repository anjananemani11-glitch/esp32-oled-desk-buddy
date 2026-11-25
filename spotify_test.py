import asyncio
import serial
import time

from winsdk.windows.media.control import (
    GlobalSystemMediaTransportControlsSessionManager as MediaManager
)

# ESP32 connection
esp32 = serial.Serial("COM5", 115200, timeout=1)

# Allow ESP32 to restart after serial connection
time.sleep(3)


async def main():

    manager = await MediaManager.request_async()

    session = manager.get_current_session()

    if session is None:

        song = "No media"
        artist = ""
        status = "NO MEDIA"

    else:

        properties = await session.try_get_media_properties_async()

        playback = session.get_playback_info()

        song = properties.title or "Unknown Song"
        artist = properties.artist or "Unknown Artist"

        status_text = str(playback.playback_status).upper()

        if "PLAYING" in status_text:
            status = "PLAYING"
        elif "PAUSED" in status_text:
            status = "PAUSED"
        else:
            status = "STOPPED"

    # Remove characters that could break serial formatting
    song = song.replace("|", "-").replace("\n", " ")
    artist = artist.replace("|", "-").replace("\n", " ")

    # Send to ESP32
    data = f"{song}|{artist}\n"
    esp32.write(data.encode("utf-8"))

    print("------ NOW PLAYING ------")
    print("Song:", song)
    print("Artist:", artist)
    print("Playback:", status)
    print("-------------------------")

    print("Sent to ESP32:", data)


async def run():

    while True:

        try:
            await main()

        except Exception as e:
            print("Error:", e)

        await asyncio.sleep(2)


try:
    asyncio.run(run())

except KeyboardInterrupt:
    print("\nDesk Buddy stopped.")

finally:
    esp32.close()