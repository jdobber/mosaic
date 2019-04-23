from pathlib import Path
import json

print("Image;Color;Rotation;Ratio;OriginalImage;InventarNr;Class;Family;Genus;Species;Scientific Name;Fundort;Datum;Gebiet;Provinz;Land;Teilkontinent;Kontinent")

pathlist = Path("/home/jens/data/cutouts").glob('**/*.json')
for path in pathlist:
    # because path is object not string
    with open(path) as json_file:
        try:
            data = json.load(json_file)
            for m in data["molluscoids"]:
                print(";".join([
                    m["imageName"],
                    m["color"],
                    str(m["angle"]),
                    str(m["ratio"]),
                    data["basename"],
                    "Gastropoda;Pleurodontidae;Papuina;vexillaris;Papuina vexillaris;Admirality Islands;;;Bismarck Archipelago;Papua New Guinea;Melanesia;Oceania"
                ]))
        except:
            pass
        