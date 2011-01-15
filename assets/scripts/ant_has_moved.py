import Ants
import random
objects = self.get_objects_in_radius()
for i in range(len(objects)):
    obj = objects[i]
    if obj.__class__ == Ants.Spice:
        print("Found a spice to eat at %s/%s" % (obj.get_node().get_x(), obj.get_node().get_y()))
        self.eat(obj.get_node())
