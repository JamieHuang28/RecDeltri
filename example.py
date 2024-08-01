import numpy as np
import time

import sys, os
# add the path to the concavehull module
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), './build/')))

from concavehull import concavehull, galaxy, skeleton, simplify_polylines, simple_join

def genPts():
    n = 100
    pts = np.random.randn(n,2)
    return pts

def getPts(file_path):
    import json
    pts = []
    with open(file_path, "r") as f:
        module_json = json.load(f)
        gdl_json = module_json["input_json"]["uss_fusion_ground_lines"]["ground_line"]["ground_line_data"]
        for gdl_json_item in gdl_json:
            local_points_fusion_json = gdl_json_item["local_points_fusion"]
            for pts_json in local_points_fusion_json:
                pts.append([pts_json["x"], pts_json["y"]])
    # turn pts into a numpy array
    pts = np.array(pts)
    return pts

def getEgoPoses(file_path):
    import json
    ego_poses = []
    with open(file_path, "r") as f:
        module_json = json.load(f)
        ego_poses_json = module_json["input_json"]["ego_poses"]
        for ego_pose_item in ego_poses_json:
            ego_poses.append([ego_pose_item["position"]["position_local"]["x"], ego_pose_item["position"]["position_local"]["y"]])
    # turn pts into a numpy array
    ego_poses = np.array(ego_poses)
    return ego_poses

def getTrajectory(file_path):
    import json
    trajectory = []
    with open(file_path, "r") as f:
        module_json = json.load(f)
        trajectory_json = module_json["output_json"]["cs_planning_result"]["trajectory"]["path"]
        for trajectory_item in trajectory_json:
            trajectory.append([trajectory_item["position_enu"]["x"], trajectory_item["position_enu"]["y"]])
    # turn pts into a numpy array
    trajectory = np.array(trajectory)
    return trajectory

def exportPts(file_name, pts):
    with open(file_name, "w") as f:
        for pt in pts:
            f.write(f"{pt[0]} {pt[1]}\n")

def procFile(file_path):
    pts = getPts(file_path)
    ego_poses = getEgoPoses(file_path)
    trajectory = getTrajectory(file_path)
    front_center = trajectory[-1]
    # print(ego_poses)
    # exportPts("random.cin", pts)

    s = time.time()
    chi_factor = 1.0
    # ch = concavehull(pts, chi_factor=chi_factor)
    ch = galaxy(pts, ego_poses[0], chi_factor=chi_factor)
    print("ch size is", len(ch))
    ch_simplified = simplify_polylines(ch, 0.2)
    print("ch_simplified size is", len(ch_simplified))
    ch_front = galaxy(pts, front_center, chi_factor=chi_factor)
    ch_simplified_front = simplify_polylines(ch_front, 0.2)
    ch_union = simple_join(ch_simplified, ch_simplified_front)
    # exportPts("galaxy.cin", ch_simplified)
    skeleton_pts = skeleton(ch_union)

    print(f"found concave hull in {time.time() - s:0.5f}s")

    try:
        import matplotlib.pyplot as plt

        plt.scatter(pts[:,0], pts[:,1])
        # plt.plot(ch[:,0], ch[:,1], 'b')
        plt.scatter(ego_poses[0][0], ego_poses[1][1], c='g')
        plt.scatter(front_center[0], front_center[1], c='y')
        plt.plot(ch_simplified[:,0], ch_simplified[:,1], 'g')
        plt.plot(ch_simplified_front[:,0], ch_simplified_front[:,1], 'y')
        plt.plot(ch_union[:,0], ch_union[:,1], '--r')
        for row in skeleton_pts:
            plt.plot([row[0], row[2]],[row[1], row[3]], '--b')
        plt.title(f"Concave Hull\nChi Factor: {chi_factor}")
        plt.show()

    except ImportError:
        print("Tried plotting, but matplotlib not found")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python example.py <path_to_input_json>")
        sys.exit(1)
    file_path = sys.argv[1]
    procFile(file_path)