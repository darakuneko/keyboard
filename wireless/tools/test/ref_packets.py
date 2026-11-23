# Reference packets from adafruit-nrfutil's own HciPacket, for tools/test/compare.mjs.
# Run with the fw venv: fw/.venv/bin/python tools/test/ref_packets.py <dfu.zip> <out.bin>
import sys, zipfile, json
from nordicsemi.dfu.dfu_transport_serial import HciPacket, DFU_START_PACKET, DFU_INIT_PACKET, DFU_DATA_PACKET, DFU_STOP_DATA_PACKET
from nordicsemi.dfu.dfu_transport import DfuTransport
from nordicsemi.dfu.util import int32_to_bytes, int16_to_bytes

z = zipfile.ZipFile(sys.argv[1])
app = json.loads(z.read('manifest.json'))['manifest']['application']
bin_, dat = z.read(app['bin_file']), z.read(app['dat_file'])
HciPacket.sequence_number = 0
pk = []
frame = [x for x in int32_to_bytes(DFU_START_PACKET)] + [x for x in int32_to_bytes(4)]
frame += [x for x in DfuTransport.create_image_size_packet(0, 0, len(bin_))]
pk.append(HciPacket(frame))
frame = [x for x in int32_to_bytes(DFU_INIT_PACKET)] + [chr(x) for x in dat] + [x for x in int16_to_bytes(0)]
pk.append(HciPacket(frame))
for i in range(0, len(bin_), 512):
    pk.append(HciPacket([x for x in int32_to_bytes(DFU_DATA_PACKET)] + [chr(x) for x in bin_[i:i + 512]]))
pk.append(HciPacket(int32_to_bytes(DFU_STOP_DATA_PACKET)))
open(sys.argv[2], 'wb').write(b''.join(bytes(p.data) for p in pk))
print(len(pk), 'packets')
