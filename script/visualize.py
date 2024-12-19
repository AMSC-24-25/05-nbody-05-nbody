import glob
import logging
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

from matplotlib.animation import FuncAnimation

log = logging.getLogger("visualize")
logging.basicConfig(format='%(asctime)s [%(levelname)s] :: %(message)s', datefmt='%m/%d/%Y %I:%M:%S %p',
                    level=logging.INFO)


def load_snapshots(nbody_filenames):
    """Load the snapshots, provided source filenames."""
    log.info(f"Loading N-Body snapshots...")
    snapshots = []
    for filename in nbody_filenames:
        data = pd.read_csv(filename)
        snapshots.append(data)
    log.info(f"N-Body snapshots loaded.")
    return snapshots


def animation_2d(snapshots, animation_interval, trajectory_length=20):
    """Visualize the N-Body evolution animation in a 2D space, provided the source snapshots."""
    log.info(f"Visualizing the N-Body snapshots...")

    # Computing velocity and min/max
    v_min, v_max = float('inf'), float('-inf')
    for snapshot in snapshots:
        snapshot['v'] = np.sqrt(snapshot['v0'] ** 2 + snapshot['v1'] ** 2)
        v_min = min(v_min, snapshot['v'].min())
        v_max = max(v_max, snapshot['v'].max())
    # Normalization function matching velocity boundaries - later velocity-color mapping
    norm = plt.Normalize(vmin=v_min, vmax=v_max)

    # Deducing domain size
    x_min, x_max = float('inf'), float('-inf')
    y_min, y_max = float('inf'), float('-inf')
    for snapshot in snapshots:
        x_min, x_max = min(x_min, snapshot['x0'].min()), max(x_max, snapshot['x0'].max())
        y_min, y_max = min(y_min, snapshot['x1'].min()), max(y_max, snapshot['x1'].max())

    # --- Plotting
    fig = plt.figure()
    ax = fig.add_subplot(111)
    # Setting labels
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    # Setting plot limits
    ax.set_xlim([x_min - 1, x_max + 1])
    ax.set_ylim([y_min - 1, y_max + 1])

    # Initializing lists to keep track of previous positions (trajectories)
    trajectories = {i: ([], []) for i in range(len(snapshots[0]))}

    # Setting-up the scatter with initial snapshot state
    scatter = ax.scatter(snapshots[0]['x0'], snapshots[0]['x1'],
                         c=snapshots[0]['v'],
                         cmap='plasma',
                         norm=norm,
                         alpha=1.0)
    scatter.set_sizes(snapshot['m'] * 10)

    # Update function for the animation
    def update(frame):
        snapshot = snapshots[frame]
        # Extracting position, mass and velocity norm
        time = snapshot['t'][0]
        mass = snapshot['m']
        x, y = snapshot['x0'], snapshot['x1']
        velocity = snapshot['v']

        # Update the trajectories: store the current (x, y) positions for each point
        for i in range(len(snapshot['x0'])):
            trajectories[i][0].append(snapshot['x0'][i])
            trajectories[i][1].append(snapshot['x1'][i])
            # If the number of stored positions exceeds the trajectory length, remove the oldest
            if len(trajectories[i][0]) > trajectory_length:
                trajectories[i][0].pop(0)
                trajectories[i][1].pop(0)

        # Plot the trajectories: Add dotted lines for each point's trajectory
        for i in range(len(snapshot['x0'])):
            ax.plot(trajectories[i][0], trajectories[i][1],
                    color=plt.cm.plasma(norm(velocity[i])),
                    alpha=0.2,
                    linewidth=0.5)

        # Updating scatter plot
        scatter.set_offsets([(row.x0, row.x1) for row in snapshot.itertuples(index=False)])
        # Setting point sizes based on mass
        scatter.set_sizes(mass * 10)
        # Mapping velocity to color
        scatter.set_array(velocity)
        ax.set_title(f"Particles position @ timestep:{frame} | time:{time}")
        return scatter,

    # Create animation
    ani = FuncAnimation(fig, update, frames=len(snapshots), interval=animation_interval, blit=False, repeat=False)

    # Show the animation
    plt.colorbar(scatter, label='Velocity')
    plt.show()
    log.info(f"N-Body visualization ended.")


def animation_3d(snapshots, animation_interval, trajectory_length=20):
    """Visualize the N-Body evolution animation in a 3D space, provided the source snapshots."""
    # Computing velocity and min/max
    v_min, v_max = float('inf'), float('-inf')
    for snapshot in snapshots:
        snapshot['v'] = np.sqrt(snapshot['v0'] ** 2 + snapshot['v1'] ** 2 + snapshot['v2'] ** 2)
        v_min = min(v_min, snapshot['v'].min())
        v_max = max(v_max, snapshot['v'].max())
    # Normalization function matching velocity boundaries - later velocity-color mapping
    norm = plt.Normalize(vmin=v_min, vmax=v_max)

    # Deducing domain size
    x_min, x_max = float('inf'), float('-inf')
    y_min, y_max = float('inf'), float('-inf')
    z_min, z_max = float('inf'), float('-inf')
    for snapshot in snapshots:
        x_min, x_max = min(x_min, snapshot['v0'].min()), max(x_max, snapshot['v0'].max())
        y_min, y_max = min(y_min, snapshot['v1'].min()), max(y_max, snapshot['v1'].max())
        z_min, z_max = min(z_min, snapshot['v2'].min()), max(z_max, snapshot['v2'].max())

    # --- Plotting
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    # Setting labels
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    # Setting plot limits
    ax.set_xlim([x_min, x_max])
    ax.set_ylim([y_min, y_max])
    ax.set_zlim([z_min, z_max])

    # Initializing lists to keep track of previous positions (trajectories)
    trajectories = {i: ([], [], []) for i in range(len(snapshots[0]))}

    # Setting-up the scatter with initial snapshot state
    scatter = ax.scatter(snapshots[0]['x0'], snapshots[0]['x1'], snapshots[0]['x2'],
                         c=snapshots[0]['v'],
                         cmap='plasma',
                         norm=norm,
                         alpha=1.0)
    scatter.set_sizes(snapshot['m'] * 10)

    # Update function for the animation
    def update(frame):
        snapshot = snapshots[frame]
        # Extracting position, mass and velocity norm
        time = snapshot['t'][0]
        mass = snapshot['m']
        x, y, z = snapshot['x0'], snapshot['x1'], snapshot['x2']
        velocity = snapshot['v']

        # Update the trajectories: store the current (x, y, z) positions for each point
        for i in range(len(snapshot['x0'])):
            trajectories[i][0].append(snapshot['x0'][i])
            trajectories[i][1].append(snapshot['x1'][i])
            trajectories[i][2].append(snapshot['x2'][i])
            # If the number of stored positions exceeds the trajectory length, remove the oldest
            if len(trajectories[i][0]) > trajectory_length:
                trajectories[i][0].pop(0)
                trajectories[i][1].pop(0)
                trajectories[i][2].pop(0)

        # Plot the trajectories: Add dotted lines for each point's trajectory
        for i in range(len(snapshot['x0'])):
            ax.plot(trajectories[i][0], trajectories[i][1], trajectories[i][2],
                    color=plt.cm.plasma(norm(velocity[i])),
                    alpha=0.2,
                    linewidth=0.5)

        # Updating scatter plot
        scatter._offsets3d = (x, y, z)
        # Setting point sizes based on mass
        scatter.set_sizes(mass * 10)
        # Mapping velocity to color
        scatter.set_array(velocity)
        ax.set_title(f"Particles position @ timestep:{frame} | time:{time}")
        return scatter,

    # Create animation
    ani = FuncAnimation(fig, update, frames=len(snapshots), interval=animation_interval, blit=False, repeat=False)

    # Show the animation
    plt.colorbar(scatter, label='Velocity')
    plt.show()


def main(input_dir, input_filename_prefix, animation_interval):
    nbody_filenames = sorted(glob.glob(f"{input_dir}/{input_filename_prefix}*.csv"))
    if not nbody_filenames:
        log.info("No CSV file found in the specified directory.")
        return

    snapshots = load_snapshots(nbody_filenames)
    if 'x2' in snapshots[0].columns:
        log.info("Interpreting the given files as a 3D problem.")
        animation_3d(snapshots, animation_interval)
    else:
        log.info("Interpreting the given files as a 2D problem.")
        animation_2d(snapshots, animation_interval)


if __name__ == "__main__":
    import argparse
    import os

    parser = argparse.ArgumentParser(description="Plot the N-Body time evolution, given some source .CSV files.")
    parser.add_argument(
        "-i",
        "--input",
        help="the directory containing the input files",
        default="output"
    )
    parser.add_argument(
        "-p",
        "--input-prefix",
        help="the prefix of the input files",
        default="nbody-"
    )
    parser.add_argument(
        "-a",
        "--animation-interval",
        help="the interval between subsequent animation steps (ms)",
        default="50"
    )
    args = parser.parse_args()

    if not os.path.exists(args.input):
        log.error(f"'{args.input}' path does not exist.")
        exit(1)

    log.info(f"-- Input directory:\t{args.input}")
    log.info(f"-- Input prefix:\t{args.input_prefix}")
    main(args.input, args.input_prefix, args.animation_interval)
