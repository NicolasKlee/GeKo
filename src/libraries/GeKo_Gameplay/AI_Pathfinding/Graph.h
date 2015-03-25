#pragma once
#include <GeKo_Gameplay/AI_Pathfinding/GraphNode.h>
#include <GeKo_Gameplay/AI_Pathfinding/Algorithm.h>
#include <GeKo_Gameplay/AI_Pathfinding/AStarNode.h>
#include "GraphNodeType.h"
#include <sstream>
#include <vector>

/** The Graph-class provides a "bucket" for GraphNodes, which cann be added to the vector-list ot the Graph. A Graph represents one closed path-system on which a AI-Unit can run.
For example one Graph could represent the search for food or the area, which the AI should protect.*/

template<class T, class A>

class Graph
{
public: 
	Graph(){
		//AStarNode* defaultNode = new AStarNode("Default");
		AStarNode* defaultNode = new AStarNode();


		AStarNode* player = new AStarNode("Player",defaultNode);
		player->setPosition(glm::vec3(0.0));
		player->setNodeType(GraphNodeType::OBJECT);
		player->setVisitor(defaultNode);
		player->setDefault(defaultNode);
		addGraphNode(player);
	}
	~Graph(){}
	
	///Adds a GraphNode-Object to the vector-list m_nodes
	/**/
	 void addGraphNode(T* node)
	 {
		 m_nodes.push_back(node);
	 }
	 
	 ///Returns the vector-list m_nodes completely
	 /**/
	 std::vector<T*>* getGraph()
	 {
		 return &m_nodes;
	 }

	 ///Returns a Node, which can be searched by its name
	 /**/
	 T* searchNode(std::string name)
	 {
		 T* returnNode;
		 for (T* s: m_nodes)
		 {
			 if (s->getName() == name)
			 {
				 returnNode = s;
			 }
		 }
		 return returnNode;
	 }

	 T* searchNode(GraphNodeType type)
	 {
		 T* returnNode;
		 for (T* s : m_nodes)
		 {
			 if (s->getNodeType() == type)
			 {
				 returnNode = s;
			 }
		 }
		 return returnNode;
	 }

	 ///Sets m_algorithm to a specific Algorithm
	 /**/
	 void setAlgorithm(A* algorithm)
	 {
		 m_algorithm = algorithm;
	 }

	 ///Returns m_algorithm
	 /**/
	 A* getAlgorithm()
	 {
		 return m_algorithm;
	 }


	 ///A Reset method to start the Algorithm on a clear Graph
	 /**The Visitor will be set bak to default, the distance travelled will be set to 0 and the temporary Distance travelled 
	 will be set to 0, too. It is important to reset the whole Graph, if you use it more than once!*/
	 void resetAlgorithm(T* defaultNode)
	 {
		 //T defaultNode("Default");
		 for (T* s: m_nodes){
			s->setVisitor(s->getDefault());
			s->setDistanceTravelled(0);
			s->setTemporary(0);
		 }
	 }

	 ///The m_distanceToGoal of the Nodes will be calculated
	 /**The calculation takes the position of the target and substract the postion of the actual Node! This will be the
	 "air-line" distance!*/
	 void calculateDistanceToGoal(T* targetNode)
	 {
		 for (T* s : m_nodes)
		 {
			 s->setDistanceToGoal(glm::length(targetNode->getPosition() - s->getPosition()));
		 }
	 }

	
	 void setExampleGraph1(glm::vec3 posPlayer)
	 {

		 //Node creation
		 //AStarNode* defaultNode = new AStarNode("Default");
		 AStarNode* defaultNode = new AStarNode();
		 AStarNode* spawn = new AStarNode("Spawn", defaultNode);
		/* spawn->setDefault(defaultNode);*/
		 AStarNode* A = new AStarNode("A", defaultNode);
		 //A->setDefault(defaultNode);
		 AStarNode* B = new AStarNode("B", defaultNode);
		 //B->setDefault(defaultNode);
		 AStarNode* Food = new AStarNode("Food", defaultNode);
		 //Food->setDefault(defaultNode);
		 AStarNode* player = new AStarNode("Player", defaultNode);
		 //player->setDefault(defaultNode);

		 spawn->setNodeType(GraphNodeType::HOME);
		 Food->setNodeType(GraphNodeType::FOOD);
		 player->setNodeType(GraphNodeType::OBJECT);

		 //Position of the nodes
		 spawn->setPosition(glm::vec3(0.0));
		 A->setPosition(glm::vec3(1.0));
		 B->setPosition(glm::vec3(3.0));
		 Food->setPosition(glm::vec3(10.0, 10.0, 0.0));
		 player->setPosition(posPlayer);

		 //Visitor settings
		/* spawn->setVisitor(defaultNode);
		 A->setVisitor(defaultNode);
		 B->setVisitor(defaultNode);
		 Food->setVisitor(defaultNode);
		 player->setVisitor(defaultNode);*/

		 //path creation //
		 Path<AStarNode>* spawnA = new Path<AStarNode>(2, spawn, A);
		 Path<AStarNode>* Aspawn = new Path<AStarNode>(2, A, spawn);
		 A->addPath(Aspawn);
		 spawn->addPath(spawnA);

		 Path<AStarNode>* spawnB = new Path<AStarNode>(3, spawn, B);
		 Path<AStarNode>* Bspawn = new Path<AStarNode>(3, B, spawn);
		 B->addPath(Bspawn);
		 spawn->addPath(spawnB);

		 Path<AStarNode>* AFood = new Path<AStarNode>(2, A, Food);
		 Path<AStarNode>* FoodA = new Path<AStarNode>(2, Food, A);
		 A->addPath(AFood);
		 Food->addPath(FoodA);

		 Path<AStarNode>* BFood = new Path<AStarNode>(15, B, Food);
		 Path<AStarNode>* FoodB = new Path<AStarNode>(15, Food, B);
		 B->addPath(BFood);
		 Food->addPath(FoodB);

		 Path<AStarNode>* AB = new Path<AStarNode>(1, A, B);
		 Path<AStarNode>* BA = new Path<AStarNode>(5, B, A);
		 A->addPath(AB);
		 B->addPath(BA);

		 //Nodes will be add to the graph
		 addGraphNode(spawn);
		 addGraphNode(A);
		 addGraphNode(B);
		 addGraphNode(Food);
		 addGraphNode(player);

		 //The Algorithm will be Set
		 AStarAlgorithm* pathfindingFood = new AStarAlgorithm("pathfindingFood");
		 setAlgorithm(pathfindingFood);
		 calculateDistanceToGoal(Food);
	 }
	 void setExampleAntAfraid(glm::vec3 posSpawn, glm::vec3 posFood, glm::vec3 posPlayer)
	 {
		//Dieser Graph enth�lt neben dem Bau auch einige FoodNodes und zwischenwegpunkte
		 //TODO: Mehr FOODs
		 //AStarNode* defaultNode = new AStarNode("Default");
		 AStarNode* defaultNode = new AStarNode();


		 AStarNode* nodeSpawn= new AStarNode("Spawn", defaultNode);
		 AStarNode* nodeB1 = new AStarNode("B1", defaultNode);
		 AStarNode* nodeB2 = new AStarNode("B2", defaultNode);
		 AStarNode* nodeW1 = new AStarNode("W1", defaultNode);
		 AStarNode* nodeW2 = new AStarNode("W2", defaultNode);
		 AStarNode* nodeE1 = new AStarNode("E1", defaultNode);
		 AStarNode* nodeE2 = new AStarNode("E2", defaultNode);
		 AStarNode* nodeFood = new AStarNode("Food", defaultNode);


		 nodeSpawn->setPosition(posSpawn);
		 nodeB1->setPosition(glm::vec3(6.0, 0.0, -3.0));
		 nodeB2->setPosition(glm::vec3(8.0, 0.0, -4.0));
		 nodeW1->setPosition(glm::vec3(6.0, 0.0, -9.0));
		 nodeW2->setPosition(glm::vec3(8.0, 0.0, -9.0));
		 nodeE1->setPosition(glm::vec3(7.0, 0.0, -12.0));
		 nodeE2->setPosition(glm::vec3(10.0, 0.0, -10.0));
		 nodeFood->setPosition(posFood);

		 nodeSpawn->setNodeType(GraphNodeType::HOME);
		 nodeFood->setNodeType(GraphNodeType::FOOD);
		 nodeB1->setNodeType(GraphNodeType::OTHER);
		 nodeB2->setNodeType(GraphNodeType::OTHER);
		 nodeW1->setNodeType(GraphNodeType::OTHER);
		 nodeW2->setNodeType(GraphNodeType::OTHER);
		 nodeE1->setNodeType(GraphNodeType::OTHER);
		 nodeE2->setNodeType(GraphNodeType::OTHER);

		/* nodeSpawn->setVisitor(defaultNode);
		 nodeB1->setVisitor(defaultNode);
		 nodeB2->setVisitor(defaultNode);
		 nodeW1->setVisitor(defaultNode);
		 nodeW2->setVisitor(defaultNode);
		 nodeE1->setVisitor(defaultNode);
		 nodeE2->setVisitor(defaultNode);
		 nodeFood->setVisitor(defaultNode);

		 nodeSpawn->setDefault(defaultNode);
		 nodeB1->setDefault(defaultNode);
		 nodeB2->setDefault(defaultNode);
		 nodeW1->setDefault(defaultNode);
		 nodeW2->setDefault(defaultNode);
		 nodeE1->setDefault(defaultNode);
		 nodeE2->setDefault(defaultNode);
		 nodeFood->setDefault(defaultNode);*/

		 Path<AStarNode>* pathSB1 = new Path<AStarNode>(1, nodeSpawn, nodeB1);
		 Path<AStarNode>* pathSW1 = new Path<AStarNode>(1, nodeSpawn, nodeW1);
		 Path<AStarNode>* pathSE1 = new Path<AStarNode>(1, nodeSpawn, nodeE1);
		 nodeSpawn->addPath(pathSB1);
		 nodeSpawn->addPath(pathSW1);
		 nodeSpawn->addPath(pathSE1);

		 Path<AStarNode>* pathB1S = new Path<AStarNode>(1, nodeB1, nodeSpawn);
		 Path<AStarNode>* pathW1S = new Path<AStarNode>(1, nodeW1, nodeSpawn);
		 Path<AStarNode>* pathE1S = new Path<AStarNode>(1, nodeE1, nodeSpawn);
		 nodeB1->addPath(pathB1S);
		 nodeW1->addPath(pathW1S);
		 nodeE1->addPath(pathE1S);

		 Path<AStarNode>* pathB1B2 = new Path<AStarNode>(6, nodeB1, nodeB2);
		 Path<AStarNode>* pathW1W2 = new Path<AStarNode>(4, nodeW1, nodeW2);
		 Path<AStarNode>* pathE1E2 = new Path<AStarNode>(2, nodeE1, nodeE2);
		 nodeB1->addPath(pathB1B2);
		 nodeW1->addPath(pathW1W2);
		 nodeE1->addPath(pathE1E2);

		 Path<AStarNode>* pathB2B1 = new Path<AStarNode>(6, nodeB2, nodeB1);
		 Path<AStarNode>* pathW2W1 = new Path<AStarNode>(4, nodeW2, nodeW1);
		 Path<AStarNode>* pathE2E1 = new Path<AStarNode>(2, nodeE2, nodeE1);
		 nodeB2->addPath(pathB2B1);
		 nodeW2->addPath(pathW2W1);
		 nodeE2->addPath(pathE2E1);

		 Path<AStarNode>* pathB2Food = new Path<AStarNode>(1, nodeB2, nodeFood);
		 Path<AStarNode>* pathW2Food = new Path<AStarNode>(1, nodeW2, nodeFood);
		 Path<AStarNode>* pathE2Food = new Path<AStarNode>(1, nodeE2, nodeFood);
		 nodeB2->addPath(pathB2Food);
		 nodeW2->addPath(pathW2Food);
		 nodeE2->addPath(pathE2Food);

		 Path<AStarNode>* pathFoodB2 = new Path<AStarNode>(1, nodeFood, nodeB2);
		 Path<AStarNode>* pathFoodW2 = new Path<AStarNode>(1, nodeFood, nodeW2);
		 Path<AStarNode>* pathFoodE2 = new Path<AStarNode>(1, nodeFood, nodeE2);
		 nodeFood->addPath(pathFoodB2);
		 nodeFood->addPath(pathFoodW2);
		 nodeFood->addPath(pathFoodE2);

		 addGraphNode(nodeSpawn);
		 addGraphNode(nodeB1);
		 addGraphNode(nodeB2);
		 addGraphNode(nodeW1);
		 addGraphNode(nodeW2);
		 addGraphNode(nodeE1);
		 addGraphNode(nodeE2);
		 addGraphNode(nodeFood);

		 AStarAlgorithm* pathfinding = new AStarAlgorithm("pathfinding");
		 setAlgorithm(pathfinding);
		 calculateDistanceToGoal(nodeSpawn);
	 }
	 void setExampleAntAggressiv(glm::vec3 posSpawn, glm::vec3 posFood, glm::vec3 posPlayer)
	 {
		 //Dieses Beispiel enth�lt nur den FoodNode an der Position des Baus und vier Nodes, welche die Ant um den Bau laufen l�sst
		 //AStarNode* nodeSpawn = new AStarNode("Spawn");
		 //AStarNode* defaultNode = new AStarNode("Default");
		 AStarNode* defaultNode = new AStarNode();

		 AStarNode* nodeLO = new AStarNode("LO",defaultNode);
		 AStarNode* nodeLU = new AStarNode("LU", defaultNode);
		 AStarNode* nodeRO = new AStarNode("RO", defaultNode);
		 AStarNode* nodeRU = new AStarNode("RU", defaultNode);
		 AStarNode* nodeFood = new AStarNode("Food", defaultNode);



		 //nodeSpawn->setPosition(posSpawn);
		 nodeLO->setPosition(glm::vec3(1.0, 0.0, 5.0));
		 nodeLU->setPosition(glm::vec3(1.0, 0.0, 1.0));
		 nodeRO->setPosition(glm::vec3(5.0, 0.0, 5.0));
		 nodeRU->setPosition(glm::vec3(5.0, 0.0, 1.0));
		 nodeFood->setPosition(posFood);

		 //nodeSpawn->setNodeType(GraphNodeType::HOME);
		 nodeFood->setNodeType(GraphNodeType::FOOD);
		 nodeLU->setNodeType(GraphNodeType::OTHER);
		 nodeLO->setNodeType(GraphNodeType::OTHER);
		 nodeRU->setNodeType(GraphNodeType::OTHER);
		 nodeRO->setNodeType(GraphNodeType::OTHER);

		 //nodeSpawn->setVisitor(defaultNode);
		 /*nodeLO->setVisitor(defaultNode);
		 nodeLU->setVisitor(defaultNode);
		 nodeRO->setVisitor(defaultNode);
		 nodeRU->setVisitor(defaultNode);
		 nodeFood->setVisitor(defaultNode);*/

		 //nodeSpawn->setDefault(defaultNode);
		 /*nodeLU->setDefault(defaultNode);
		 nodeLO->setDefault(defaultNode);
		 nodeRU->setDefault(defaultNode);
		 nodeRO->setDefault(defaultNode);
		 nodeFood->setDefault(defaultNode);*/

		 Path<AStarNode>* pathLORO = new Path<AStarNode>(1, nodeLO, nodeRO);
		 Path<AStarNode>* pathLOF = new Path<AStarNode>(1, nodeLO, nodeFood);
		 Path<AStarNode>* pathLOLU = new Path<AStarNode>(1, nodeLO, nodeLU);
		 nodeLO->addPath(pathLORO);
		 nodeLO->addPath(pathLOF);
		 nodeLO->addPath(pathLOLU);

		 Path<AStarNode>* pathLULO = new Path<AStarNode>(1, nodeLU, nodeLO);
		 Path<AStarNode>* pathLUF = new Path<AStarNode>(1, nodeLU, nodeFood);
		 Path<AStarNode>* pathLURU = new Path<AStarNode>(1, nodeLU, nodeRU);
		 nodeLU->addPath(pathLULO);
		 nodeLU->addPath(pathLUF);
		 nodeLU->addPath(pathLURU);

		 Path<AStarNode>* pathRULU = new Path<AStarNode>(6, nodeRU, nodeLU);
		 Path<AStarNode>* pathRUF = new Path<AStarNode>(4, nodeRU, nodeFood);
		 Path<AStarNode>* pathRURO = new Path<AStarNode>(2, nodeRU, nodeRO);
		 nodeRU->addPath(pathRULU);
		 nodeRU->addPath(pathRUF);
		 nodeRU->addPath(pathRURO);

		 Path<AStarNode>* pathROLO = new Path<AStarNode>(6, nodeRO, nodeLO);
		 Path<AStarNode>* pathROF = new Path<AStarNode>(4, nodeRO, nodeFood);
		 Path<AStarNode>* pathRORU = new Path<AStarNode>(2, nodeRO, nodeRU);
		 nodeRO->addPath(pathROLO);
		 nodeRO->addPath(pathROF);
		 nodeRO->addPath(pathRORU);

		 Path<AStarNode>* pathFLO = new Path<AStarNode>(1, nodeFood, nodeLO);
		 Path<AStarNode>* pathFLU = new Path<AStarNode>(1, nodeFood, nodeLU);
		 Path<AStarNode>* pathFRO = new Path<AStarNode>(1, nodeFood, nodeRO);
		 Path<AStarNode>* pathFRU = new Path<AStarNode>(1, nodeFood, nodeRU);
		 nodeFood->addPath(pathFLO);
		 nodeFood->addPath(pathFLU);
		 nodeFood->addPath(pathFRO);
		 nodeFood->addPath(pathFRU);

		 //addGraphNode(nodeSpawn);
		 addGraphNode(nodeLU);
		 addGraphNode(nodeLO);
		 addGraphNode(nodeRO);
		 addGraphNode(nodeRU);
		 addGraphNode(nodeFood);
		 

		 AStarAlgorithm* pathfinding = new AStarAlgorithm("pathfinding");
		 setAlgorithm(pathfinding);
		 calculateDistanceToGoal(nodeFood);
	 }

	 void generateForest(std::vector<glm::vec3> treePositions,AStarNode *nodeSpawn, AStarNode *defaultNode){
		 //int i = 1;
		 std::stringstream name;
		 for (int i = 0; i < treePositions.size(); i++){
		 //for (glm::vec3 position : treePositions){
			 name << "Tree" << i;
			 //std::string nametmp = name.str();
			 AStarNode* tree = new AStarNode(name.str(), defaultNode, treePositions.at(i), GraphNodeType::FOOD);
			 Path<AStarNode>* pathSpawnTree = new Path<AStarNode>(1, nodeSpawn, tree);
			 nodeSpawn->addPath(pathSpawnTree);
			 Path<AStarNode>* pathTreeSpawn = new Path<AStarNode>(1, tree, nodeSpawn);
			 tree->addPath(pathTreeSpawn);
			 addGraphNode(tree);
			 name.str("");
			 //i++;

		 }
	 }

	 void setExampleAntAfraid2(glm::vec3 posSpawn, glm::vec3 posPlayer)
	 {
		 //Dieser Graph enth�lt neben dem Bau auch einige FoodNodes und zwischenwegpunkte
		 //TODO: Mehr FOODs
		 //AStarNode* defaultNode = new AStarNode("Default");
		 AStarNode* defaultNode = new AStarNode();

		 AStarNode* nodeSpawn = new AStarNode("Spawn", defaultNode, posSpawn, GraphNodeType::HOME);
		 //glm::vec3 trees[5] = { glm::vec3(16.0, 0.0, 76.0), glm::vec3(23.0, 0.0, 74.0), glm::vec3(21.5, 0.0, 78.5), glm::vec3(15.0, 0.0, 81.5), glm::vec3(27.5, 0.0, 88.5), };
		 std::vector<glm::vec3> trees;
		 trees.push_back(glm::vec3(16.0, 0.0, 76.0));
		 trees.push_back(glm::vec3(23.0, 0.0, 74.0));
		 trees.push_back(glm::vec3(21.5, 0.0, 78.5));
		 trees.push_back(glm::vec3(15.0, 0.0, 81.5));
		 trees.push_back(glm::vec3(27.5, 0.0, 88.5));
		 //glm::vec3 trees[5] = { glm::vec3(60.0, 0.0, 760.0), glm::vec3(130.0, 0.0, 740.0), glm::vec3(115.0, 0.0, 785.0), glm::vec3(50.0, 0.0, 815.0), glm::vec3(175.0, 0.0, 885.0), }
		 generateForest(trees, nodeSpawn, defaultNode);
		 /*AStarNode* nodeB1 = new AStarNode("B1", defaultNode);
		 AStarNode* nodeB2 = new AStarNode("B2", defaultNode);
		 AStarNode* nodeW1 = new AStarNode("W1", defaultNode);
		 AStarNode* nodeW2 = new AStarNode("W2", defaultNode);
		 AStarNode* nodeE1 = new AStarNode("E1", defaultNode);
		 AStarNode* nodeE2 = new AStarNode("E2", defaultNode);
		 AStarNode* nodeFood = new AStarNode("Food", defaultNode);*/


		 //nodeSpawn->setPosition(posSpawn);
		 //nodeB1->setPosition(glm::vec3(6.0, 0.0, -3.0));
		 //nodeB2->setPosition(glm::vec3(8.0, 0.0, -4.0));
		 //nodeW1->setPosition(glm::vec3(6.0, 0.0, -9.0));
		 //nodeW2->setPosition(glm::vec3(8.0, 0.0, -9.0));
		 //nodeE1->setPosition(glm::vec3(7.0, 0.0, -12.0));
		 //nodeE2->setPosition(glm::vec3(10.0, 0.0, -10.0));
		 //nodeFood->setPosition(posFood);

		 /*nodeSpawn->setNodeType(GraphNodeType::HOME);
		 nodeFood->setNodeType(GraphNodeType::FOOD);
		 nodeB1->setNodeType(GraphNodeType::OTHER);
		 nodeB2->setNodeType(GraphNodeType::OTHER);
		 nodeW1->setNodeType(GraphNodeType::OTHER);
		 nodeW2->setNodeType(GraphNodeType::OTHER);
		 nodeE1->setNodeType(GraphNodeType::OTHER);
		 nodeE2->setNodeType(GraphNodeType::OTHER);*/


		/* nodeSpawn->setVisitor(defaultNode);
		 nodeB1->setVisitor(defaultNode);
		 nodeB2->setVisitor(defaultNode);
		 nodeW1->setVisitor(defaultNode);
		 nodeW2->setVisitor(defaultNode);
		 nodeE1->setVisitor(defaultNode);
		 nodeE2->setVisitor(defaultNode);
		 nodeFood->setVisitor(defaultNode);

		 nodeSpawn->setDefault(defaultNode);
		 nodeB1->setDefault(defaultNode);
		 nodeB2->setDefault(defaultNode);
		 nodeW1->setDefault(defaultNode);
		 nodeW2->setDefault(defaultNode);
		 nodeE1->setDefault(defaultNode);
		 nodeE2->setDefault(defaultNode);
		 nodeFood->setDefault(defaultNode);*/

		 /*Path<AStarNode>* pathSB1 = new Path<AStarNode>(1, nodeSpawn, nodeB1);
		 Path<AStarNode>* pathSW1 = new Path<AStarNode>(1, nodeSpawn, nodeW1);
		 Path<AStarNode>* pathSE1 = new Path<AStarNode>(1, nodeSpawn, nodeE1);
		 nodeSpawn->addPath(pathSB1);
		 nodeSpawn->addPath(pathSW1);
		 nodeSpawn->addPath(pathSE1);

		 Path<AStarNode>* pathB1S = new Path<AStarNode>(1, nodeB1, nodeSpawn);
		 Path<AStarNode>* pathW1S = new Path<AStarNode>(1, nodeW1, nodeSpawn);
		 Path<AStarNode>* pathE1S = new Path<AStarNode>(1, nodeE1, nodeSpawn);
		 nodeB1->addPath(pathB1S);
		 nodeW1->addPath(pathW1S);
		 nodeE1->addPath(pathE1S);

		 Path<AStarNode>* pathB1B2 = new Path<AStarNode>(6, nodeB1, nodeB2);
		 Path<AStarNode>* pathW1W2 = new Path<AStarNode>(4, nodeW1, nodeW2);
		 Path<AStarNode>* pathE1E2 = new Path<AStarNode>(2, nodeE1, nodeE2);
		 nodeB1->addPath(pathB1B2);
		 nodeW1->addPath(pathW1W2);
		 nodeE1->addPath(pathE1E2);

		 Path<AStarNode>* pathB2B1 = new Path<AStarNode>(6, nodeB2, nodeB1);
		 Path<AStarNode>* pathW2W1 = new Path<AStarNode>(4, nodeW2, nodeW1);
		 Path<AStarNode>* pathE2E1 = new Path<AStarNode>(2, nodeE2, nodeE1);
		 nodeB2->addPath(pathB2B1);
		 nodeW2->addPath(pathW2W1);
		 nodeE2->addPath(pathE2E1);

		 Path<AStarNode>* pathB2Food = new Path<AStarNode>(1, nodeB2, nodeFood);
		 Path<AStarNode>* pathW2Food = new Path<AStarNode>(1, nodeW2, nodeFood);
		 Path<AStarNode>* pathE2Food = new Path<AStarNode>(1, nodeE2, nodeFood);
		 nodeB2->addPath(pathB2Food);
		 nodeW2->addPath(pathW2Food);
		 nodeE2->addPath(pathE2Food);

		 Path<AStarNode>* pathFoodB2 = new Path<AStarNode>(1, nodeFood, nodeB2);
		 Path<AStarNode>* pathFoodW2 = new Path<AStarNode>(1, nodeFood, nodeW2);
		 Path<AStarNode>* pathFoodE2 = new Path<AStarNode>(1, nodeFood, nodeE2);
		 nodeFood->addPath(pathFoodB2);
		 nodeFood->addPath(pathFoodW2);
		 nodeFood->addPath(pathFoodE2);*/

		 addGraphNode(nodeSpawn);
		 /*addGraphNode(nodeB1);
		 addGraphNode(nodeB2);
		 addGraphNode(nodeW1);
		 addGraphNode(nodeW2);
		 addGraphNode(nodeE1);
		 addGraphNode(nodeE2);
		 addGraphNode(nodeFood);*/

		 AStarAlgorithm* pathfinding = new AStarAlgorithm("pathfinding");
		 setAlgorithm(pathfinding);
		 calculateDistanceToGoal(nodeSpawn);
	 }

protected:
	  std::vector<T*> m_nodes;

	  A* m_algorithm;
};