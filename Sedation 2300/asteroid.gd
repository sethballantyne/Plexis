extends Area2D

var explosion_scene = preload("res://explosion.tscn")
var move_speed = 100.0
var score_emitted = false

signal score
signal spawned

# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	move_speed = rand_range(75, 250) * 1.0


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	position -= Vector2(move_speed * delta, 0.0)
	if position.x <= -100:
		queue_free()

func _on_asteroid_area_entered(area):
	if area.is_in_group("shot") or area.is_in_group("player"):
		if not score_emitted:
			score_emitted = true
			if area.is_in_group("shot"): 
				# only increment score if the laser hit the asteroid
				emit_signal("score")
			queue_free()
			
			var stage_node = get_parent()
			var explosion_instance = explosion_scene.instance()
			explosion_instance.position = position
			stage_node.add_child(explosion_instance)
