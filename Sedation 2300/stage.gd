extends Node2D

var game_over = false
var asteroid = preload("res://asteroid.tscn")
var score = 0

const SCREEN_WIDTH = 320
const SCREEN_HEIGHT = 180

# Declare member variables here. Examples:
# var a = 2
# var b = "text"
func _ready():
	get_node("player").connect("player_destroyed", self, "_on_player_destroyed")
	get_node("background_and_asteroids/asteroid").connect("spawned", self, "_on_asteroid_spawned")
	get_node("background_and_asteroids").gameStarted = true
	

func _input(event):
	if Input.is_key_pressed(KEY_ESCAPE):
		get_tree().quit()
	if game_over and Input.is_key_pressed(KEY_ENTER):
		get_tree().change_scene("res://stage.tscn")
	
func _on_player_destroyed():
	get_node("ui/retry").show()
	game_over = true

func _on_player_score():
	score += 1
	get_node("ui/score").text = "Score: " + str(score)
	
	
func _on_spawn_timer_timeout():
	#var asteroid_instance = asteroid.instance()
	#asteroid_instance.position = Vector2(SCREEN_WIDTH + 8, rand_range(0, SCREEN_HEIGHT))
	#asteroid_instance.connect("score", self, "_on_player_score")
	#add_child(asteroid_instance)
	pass
	
	
