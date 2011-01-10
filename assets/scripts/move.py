import Ants
import random
# print("Hi, i'm %s, I need a target" % (self.get_id(), ))
current_node = self.get_node()
neighbours = nodeManager.get_neighbour_nodes(current_node)
target = random.choice(neighbours)
self.set_move_target(target)
# print("Target found: %s/%s" % (target.get_coordinate().get_x(), target.get_coordinate().get_y()))
