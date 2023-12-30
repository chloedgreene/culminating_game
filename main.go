package main

import rl "github.com/gen2brain/raylib-go/raylib"

func main() {
	rl.InitWindow(800, 450, "Culmiating Game")
	defer rl.CloseWindow()

	camera := rl.Camera3D{}
	camera.Position = rl.NewVector3(4.0, 2.0, 4.0)
	camera.Target = rl.NewVector3(0.0, 1.8, 0.0)
	camera.Up = rl.NewVector3(0.0, 1.0, 0.0)
	camera.Fovy = 60.0
	camera.Projection = rl.CameraPerspective

	cubePosition := rl.NewVector3(0.0, 0.0, 0.0)

	rl.SetTargetFPS(60)

	for !rl.WindowShouldClose() {
		rl.UpdateCamera(&camera, rl.CameraFirstPerson)

		rl.BeginDrawing()

		rl.ClearBackground(rl.RayWhite)

		rl.BeginMode3D(camera)
			rl.DrawCube(cubePosition, 2.0, 2.0, 2.0, rl.Red)
			rl.DrawCubeWires(cubePosition, 2.0, 2.0, 2.0, rl.Maroon)
			rl.DrawGrid(10, 1.0)
		rl.EndMode3D()

		rl.EndDrawing()


	}
}


