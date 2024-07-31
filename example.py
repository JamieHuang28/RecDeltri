import numpy as np
import time

import sys, os
# add the path to the concavehull module
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), './build/')))

from concavehull import concavehull, galaxy

def genPts():
    n = 100
    pts = np.random.randn(n,2)
    return pts

def getPts():
    import json
    pts = []
    with open("100_module_test.json", "r") as f:
        module_json = json.load(f)
        gdl_json = module_json["input_json"]["uss_fusion_ground_lines"]["ground_line"]["ground_line_data"]
        for gdl_json_item in gdl_json:
            local_points_fusion_json = gdl_json_item["local_points_fusion"]
            for pts_json in local_points_fusion_json:
                pts.append([pts_json["x"], pts_json["y"]])
    # turn pts into a numpy array
    pts = np.array(pts)
    return pts

def getEgoPoses():
    import json
    ego_poses = []
    with open("100_module_test.json", "r") as f:
        module_json = json.load(f)
        ego_poses_json = module_json["input_json"]["ego_poses"]
        for ego_pose_item in ego_poses_json:
            ego_poses.append([ego_pose_item["position"]["position_local"]["x"], ego_pose_item["position"]["position_local"]["y"]])
    # turn pts into a numpy array
    ego_poses = np.array(ego_poses)
    return ego_poses

def exportPts(file_name, pts):
    with open(file_name, "w") as f:
        for pt in pts:
            f.write(f"{pt[0]} {pt[1]}\n")

pts = getPts()
ego_poses = getEgoPoses()
# print(ego_poses)
# exportPts("random.cin", pts)

s = time.time()
chi_factor = 1.0
# ch = concavehull(pts, chi_factor=chi_factor)
ch = galaxy(pts, ego_poses[0], chi_factor=chi_factor)

print(f"found concave hull in {time.time() - s:0.5f}s")

try:
    import matplotlib.pyplot as plt

    plt.scatter(pts[:,0], pts[:,1])
    plt.plot(ch[:,0], ch[:,1], 'g')
    plt.title(f"Concave Hull\nChi Factor: {chi_factor}")
    plt.show()

except ImportError:
    print("Tried plotting, but matplotlib not found")
