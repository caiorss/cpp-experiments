from tbox import Runstat 
rs = Runstat(); 
rs.add(20.5); rs.add(10.0); rs.add(5); rs.add(8); rs.add(-10);
rs.add(8.5);  rs.add(5.245); rs.add(-14.0);

print(" => [PYTHON] Evaluating file pyembed_script.py")
print(' => mean = ', rs.mean())
print(' => sdev = ', rs.sdev())
print(' => size = ', rs.size())

import os

for dir in os.listdir("/etc/"):
    print(" => ", dir)
