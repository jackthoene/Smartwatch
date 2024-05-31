from flask import Flask, Response, jsonify
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_agg import FigureCanvasAgg as FigureCanvas
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import io
import json
from flask_cors import CORS

app = Flask(__name__)
CORS(app)


def load_imu_data():
    with open("imu_data.json", "r") as file:
        data = json.load(file)
    return data


def create_pyramid_frame(imu_data, frame):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection="3d")

    vertices = np.array(
        [[0, 0, 1], [-0.5, -0.5, 0], [-0.5, 0.5, 0], [0.5, 0.5, 0], [0.5, -0.5, 0]]
    )

    faces = [
        [vertices[0], vertices[1], vertices[2]],
        [vertices[0], vertices[2], vertices[3]],
        [vertices[0], vertices[3], vertices[4]],
        [vertices[0], vertices[4], vertices[1]],
        [vertices[1], vertices[2], vertices[3], vertices[4]],
    ]

    def rotate_point(point, pitch, yaw, roll):
        pitch_matrix = np.array(
            [
                [1, 0, 0],
                [0, np.cos(pitch), -np.sin(pitch)],
                [0, np.sin(pitch), np.cos(pitch)],
            ]
        )
        yaw_matrix = np.array(
            [[np.cos(yaw), 0, np.sin(yaw)], [0, 1, 0], [-np.sin(yaw), 0, np.cos(yaw)]]
        )
        roll_matrix = np.array(
            [
                [np.cos(roll), -np.sin(roll), 0],
                [np.sin(roll), np.cos(roll), 0],
                [0, 0, 1],
            ]
        )

        rotated_point = np.dot(pitch_matrix, point)
        rotated_point = np.dot(yaw_matrix, rotated_point)
        rotated_point = np.dot(roll_matrix, rotated_point)

        return rotated_point

    ax.clear()
    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(0, 1)

    pitch = imu_data["x"][frame]
    yaw = imu_data["y"][frame]
    roll = imu_data["z"][frame]

    rotated_faces = []
    for face in faces:
        rotated_face = [rotate_point(vertex, pitch, yaw, roll) for vertex in face]
        rotated_faces.append(rotated_face)

    face_normals = [
        np.cross(rotated_face[1] - rotated_face[0], rotated_face[2] - rotated_face[0])
        for rotated_face in rotated_faces
    ]

    avg_z_coords = [np.mean([vertex[2] for vertex in face]) for face in rotated_faces]
    sorted_indices = np.argsort(avg_z_coords)

    for i in sorted_indices:
        face = np.array(rotated_faces[i])
        normal = face_normals[i]
        ax.add_collection3d(
            Poly3DCollection(
                [face],
                color="blue",
                alpha=0.5,
                linewidths=1,
                edgecolors="black",
                facecolors=plt.cm.plasma(normal[2]),
            )
        )

    output = io.BytesIO()
    FigureCanvas(fig).print_png(output)
    plt.close(fig)  # Close the figure after rendering
    return output.getvalue()


@app.route("/animation")
def stream_animation():
    imu_data = load_imu_data()
    frame = -1  # Use the last frame for the animation
    image = create_pyramid_frame(imu_data, frame)
    return Response(image, mimetype="image/png")


@app.route("/imu-data")
def get_imu_data():
    imu_data = load_imu_data()
    return jsonify(imu_data)


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
