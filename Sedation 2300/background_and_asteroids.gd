extends Node2D

var asteroid = preload("res://asteroid.tscn")
const SCREEN_WIDTH = 320
const SCREEN_HEIGHT = 180

var gameStarted = false
var test_score = 0

var rng = RandomNumberGenerator.new()
# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	get_node("spawn_asteroid_timer").connect("timeout", self, "_on_spawn_asteroid_timer_timeout")
	rng.randomize()
	
func _on_player_score():
	test_score += 1
	get_tree().get_root().get_node("stage/ui/score").text = "Score: " + str(test_score)
	#get_node("../stage/ui/score").text = "Score: " + str(score)
	
func _on_spawn_asteroid_timer_timeout():
	var asteroid_instance = asteroid.instance()
	
	# admittedly, this shouldn't be here. I'm still getting my head
	# around how Godot structures everything
	if gameStarted:
		asteroid_instance.connect("score", self, "_on_player_score")
	
	#####################################################################
	## Determine whether we're rotating the asteroid
	#####################################################################
	var temp = rng.randi_range(1, 5)
	if temp == 2: # 2 == rotate this motherfucker!
		var temp_rotation_speed = rand_range(1, 8) * 0.001
		
		#rotate the asteroid clockwise by default
		asteroid_instance.rotation_speed = temp_rotation_speed
		
		temp = rng.randi_range(1, 2)
		if temp == 2:
			# fuck that, we're going ANTI-CLOCKWISE this time!
			asteroid_instance.rotation_speed = -asteroid_instance.rotation_speed

	###################################################################
	## End rotation shite
	###################################################################
	asteroid_instance.position = Vector2(SCREEN_WIDTH + 8, rand_range(0, SCREEN_HEIGHT))
	add_child(asteroid_instance)
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
