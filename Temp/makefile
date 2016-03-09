CC:=g++ -std=c++11 -c 
CO:=g++ -std=c++11 -o 
BACK:= -lGL -lGLU -lglut
BIN:= ./bin
HEADER = -I /home/kaima
OBJ:= main.o AABB.o Link.o Contact.o ForceGenerator.o PhysicsWorld.o Polygon.o MainScene.o Circle.o Rigidbody.o Tool.o Collision.o Vector.o Matrix.o

main:$(OBJ)
	$(CO) main $(OBJ) $(BACK)
main.o:main.cpp
	$(CC) main.cpp $(HEADER)
Collision.o:Collision.cpp
	$(CC) Collision.cpp
Vector.o:Math/Vector.cpp
	$(CC) Math/Vector.cpp $(HEADER)
Matrix.o:Math/Matrix.cpp
	$(CC) Math/Matrix.cpp $(HEADER)
Tool.o:Tool.cpp
	$(CC) Tool.cpp
Rigidbody.o:Rigidbody.cpp
	$(CC) Rigidbody.cpp
Circle.o:Circle.cpp
	$(CC) Circle.cpp
Polygon.o:Polygon.cpp
	$(CC) Polygon.cpp
MainScene.o:MainScene.cpp
	$(CC) MainScene.cpp
PhysicsWorld.o:PhysicsWorld.cpp
	$(CC) PhysicsWorld.cpp
ForceGenerator.o:ForceGenerator.cpp
	$(CC) ForceGenerator.cpp
Contact.o:Contact.cpp
	$(CC) Contact.cpp
AABB.o:AABB.cpp
	$(CC) AABB.cpp

Link.o:Link.cpp
	$(CC) Link.cpp

clean:
	rm main *.o