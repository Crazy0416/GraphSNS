#include "GraphItem.h"

void _ReheapUp(vector<NodeItem*> v, int root, int last)
{

	int parent = (last - 1) / 2;

	if (last > root)
	{
		if (*v[parent] > *v[last])
		{
			swap(v[parent], v[last]);
			_ReheapUp(v, root, parent);
		}
	}
}
void _ReheapDown(vector<NodeItem*> v, int root, int last)
{

	int minChild;
	int rightChild = root * 2 + 2;
	int leftChild = root * 2 + 1;

	if (leftChild <= last)
	{
		if (leftChild == last)
			minChild = leftChild;
		else
		{
			if (*v[leftChild] > *v[rightChild])
				minChild = rightChild;
			else
				minChild = leftChild;
		}

		if (*v[root] > *v[minChild])
		{
			swap(v[root], v[minChild]);
			_ReheapDown(v, minChild, last);
		}
	}
}

void CoauthorGraphItem::ReheapUp(vector<int> v, int root, int last)
{

	int parent = (last - 1) / 2;

	if (last > root)
	{
		if (*nodeList[v[parent]] < *nodeList[v[last]])
		{
			swap(v[parent], v[last]);
			ReheapUp(v, root, parent);
		}
	}
}
void CoauthorGraphItem::ReheapDown(vector<int> v, int root, int last)
{

	int maxChild;
	int rightChild = root * 2 + 2;
	int leftChild = root * 2 + 1;

	if (leftChild <= last)
	{
		if (leftChild == last)
			maxChild = leftChild;
		else
		{
			if (*nodeList[v[leftChild]] < *nodeList[v[rightChild]])
				maxChild = rightChild;
			else
				maxChild = leftChild;
		}

		if (*nodeList[v[root]] < *nodeList[v[maxChild]])
		{
			swap(v[root], v[maxChild]);
			ReheapDown(v, maxChild, last);
		}
	}
}

CoauthorGraphItem::CoauthorGraphItem(ifstream& fin)
{
	if (!fin)
		throw std::exception("coauthor graph file input is invalid");

	/**
	*	Parse Coauthor dataset
	*	- author1, author2 publish_year
	*	Column Delimiter:		||
	*/
	std::string line;
	vector<std::string> tokens;

	node_cnt = 0;
	qDebug() << "* couathor graph reading start";

	//한 줄씩 읽어서 Parse
	while (std::getline(fin, line) && !line.empty()) {
		line_cnt++;

		//boost::split 이용해 문자열 분리
		//tokens[0]: Author1
		//tokens[1]: Author2
		//tokens[2]: Published year.
		boost::split(tokens, line, boost::is_any_of("||"), boost::token_compress_on);

		const string& author1 = tokens[0];
		const string& author2 = tokens[1];
		if (node_ids.left.find(author1) == node_ids.left.end()) {
			node_ids.insert(bm_type::value_type(author1, node_cnt++));
		}

		if (node_ids.left.find(author2) == node_ids.left.end()) {
			node_ids.insert(bm_type::value_type(author2, node_cnt++));
		}

		edges.push_back(pair<string, string>(author1, author2));

		//debug
		if (node_cnt > NODE_LIMIT) break;
	}
	qDebug() << "* coauthor graph reading complete";
	qDebug() << "* # of nodes: " << node_cnt;
	qDebug() << "* # of edges: " << edges.size();

	//edge conversion
	//<string, string> to <int, int>
	//using boost::bimap (bidirectional map)
	for (auto edge : edges) {
		edges_indexes.push_back({
			node_ids.left.find(edge.first)->get_right(),
			node_ids.left.find(edge.second)->get_right()
		});
	}
	//Graph --> defined in "PaperGraphWidget.h"
	//Graph graph(edges_indexes.begin(), edges_indexes.end(), node_ids.size());
	graph = new Graph(edges_indexes.begin(), edges_indexes.end(), node_ids.size());
	
	//set index property
	qDebug() << "* set vertex property start";
	typedef typename graph_traits<Graph>::edge_iterator edge_iterator;
	typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
	vertex_iterator vi, vi_end;
	int i = 0;
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi) {
		//Vertex Property 설정
		//index: 0 ~ ...
		//name : map의 value(i) 기준으로 찾은 Key
		//		map --> map<string, int> (boost bidirectional map)
		boost::put(vertex_index, *graph, *vi, i);
		boost::put(vertex_name, *graph, *vi,
			node_ids.right.find(i)->get_left());

		++i;
	}
	qDebug() << "* set vertex property end";

	qDebug() << "* make graph layout start";
	typedef square_topology<> Topology;
	boost::minstd_rand gen;
	Topology topology(gen, (double)SCREEN_SIZE);
	Topology::point_type origin;
	origin[0] = origin[1] = (double)SCREEN_SIZE;
	Topology::point_difference_type extent;
	extent[0] = extent[1] = (double)SCREEN_SIZE;
	rectangle_topology<> rect_top(gen,
		-SCREEN_SIZE / 2, -SCREEN_SIZE / 2,
		SCREEN_SIZE / 2, SCREEN_SIZE / 2);

	switch (LAYOUT_MODE) {
	case GRAPH_LAYOUT::RANDOM_LAYOUT:
		random_graph_layout(*graph, get(vertex_position, *graph), rect_top);
		break;

	case GRAPH_LAYOUT::CIRCLE_LAYOUT:
		circle_graph_layout(*graph, get(vertex_position, *graph), SCREEN_SIZE / 2);
		break;

	case GRAPH_LAYOUT::FRUCHTERMAN_REINGOLD_LAYOUT:
		fruchterman_reingold_force_directed_layout(*graph,
			get(vertex_position, *graph),
			topology,
			attractive_force(square_distance_attractive_force())
			.cooling(linear_cooling<double>(50))
		);
		break;
	}
	qDebug() << "* make graph layout end";

	// vertex info(참여도) initialization  TODO : 수정중@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@2
	vector<int> vInfo;
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi)
		vInfo.push_back(0);

	vector<vector<std::string>> vcoauthor;		// vertex coauthor(노드 공동 작업자 이름 벡터)
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi)
	{
		vector<std::string> tmp;
		vcoauthor.push_back(tmp);
	}

	//add edges
	typedef square_topology<> Topology;
	typedef typename Topology::point_type Point;
	auto position = get(vertex_position, *graph);
	auto index = get(vertex_index, *graph);
	auto label = get(vertex_name, *graph);

	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typename graph_traits<Graph>::edge_iterator ei, ei_end;
	vertex_descriptor u, v;
	for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei) {
		// vertex info set
		i = index[ei->m_source];
		vInfo[i]++;

		vcoauthor[i].push_back(label[ei->m_target]);		// coauthor 이름 벡터에 넣음

		i = index[ei->m_target];
		vInfo[i]++;
		vcoauthor[i].push_back(label[ei->m_source]);


		u = source(*ei, *graph);
		v = target(*ei, *graph);
		Point p1 = position[u];
		Point p2 = position[v];

		//make edge item and push it to list
		EdgeItem *edge;
		edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::black), 0, index[ei->m_source], index[ei->m_target]);
		edge->setPos(QPointF(0, 0));
	
		edgeList << edge;
	}
	i = 0;
	//add nodes
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi) {
		Point p = position[*vi];
		std::string name = label[*vi];

		//make node item and push it to list
		NodeItem *node;
		node = new NodeItem(p[0], p[1], QColor(Qt::green), QString(name.c_str()), vInfo[i], vcoauthor[i]);
		node->setPos(QPointF(0, 0));
		nodeList << node;
		i++;
	}
}

CoauthorGraphItem::CoauthorGraphItem(CoauthorGraphItem& src)
	: GraphItem(src.edges, src.node_ids, src.edges_indexes, src.node_cnt, src.line_cnt)
{
	this->graph = src.graph;			// Not deep copy!!!!!!
	//nodeList copy
	for (auto& item : src.nodeList) {
		NodeItem* dump = new NodeItem(*item);
		this->nodeList << dump;
	}
	// EdgeList copy
	for (auto& item : src.edgeList) {
		EdgeItem* dump = new EdgeItem(*item);
		this->edgeList << dump;
	}
	// minHeap copy
	for (auto& item : src.minHeap) {
		NodeItem* dump = new NodeItem(*item);
		this->minHeap.push_back(dump);
	}
}

QStringList* CoauthorGraphItem::updateGraph(ifstream& fin)
{
	QStringList* updateCoauthorList = new QStringList();
	if (!fin)
		throw std::exception("coauthor graph file input is invalid");

	/**
	*	Parse Coauthor dataset
	*	- author1, author2 publish_year
	*	Column Delimiter:		||
	*/
	std::string line;
	vector<std::string> tokens;

	qDebug() << "* coauthor graph update start";

	int local_line_cnt = 0;

	//한 줄씩 읽어서 Parse
	while (std::getline(fin, line) && !line.empty()) {

		local_line_cnt++;

		if (local_line_cnt > line_cnt) {
			
			line_cnt++;

			//boost::split 이용해 문자열 분리
			//tokens[0]: Author1
			//tokens[1]: Author2
			//tokens[2]: Published year.
			boost::split(tokens, line, boost::is_any_of("||"), boost::token_compress_on);

			const string& author1 = tokens[0];
			const string& author2 = tokens[1];
			updateCoauthorList->append(QString::fromStdString(author1));
			updateCoauthorList->append(QString::fromStdString(author2));

			if (node_ids.left.find(author1) == node_ids.left.end()) {
				node_ids.insert(bm_type::value_type(author1, node_cnt++));
			}

			if (node_ids.left.find(author2) == node_ids.left.end()) {
				node_ids.insert(bm_type::value_type(author2, node_cnt++));
			}

			edges.push_back(pair<string, string>(author1, author2));
		}

		//debug
		if (node_cnt > NODE_LIMIT) break;

	}
	qDebug() << "* coauthor graph update complete";
	qDebug() << "* # of nodes: " << node_cnt;
	qDebug() << "* # of edges: " << edges.size();

	//edge conversion
	//<string, string> to <int, int>
	//using boost::bimap (bidirectional map)
	for (auto edge : edges) {
		edges_indexes.push_back({
			node_ids.left.find(edge.first)->get_right(),
			node_ids.left.find(edge.second)->get_right()
		});
	}
	//Graph --> defined in "PaperGraphWidget.h"
	//Graph graph(edges_indexes.begin(), edges_indexes.end(), node_ids.size());
	graph = new Graph(edges_indexes.begin(), edges_indexes.end(), node_ids.size());

	//set index property
	
	typedef typename graph_traits<Graph>::edge_iterator edge_iterator;
	typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
	vertex_iterator vi, vi_end;
	int i = 0;
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi) {
		//Vertex Property 설정
		//index: 0 ~ ...
		//name : map의 value(i) 기준으로 찾은 Key
		//		map --> map<string, int> (boost bidirectional map)
		boost::put(vertex_index, *graph, *vi, i);
		boost::put(vertex_name, *graph, *vi,
			node_ids.right.find(i)->get_left());

		++i;
	}
	
	typedef square_topology<> Topology;
	boost::minstd_rand gen;
	Topology topology(gen, (double)SCREEN_SIZE);
	Topology::point_type origin;
	origin[0] = origin[1] = (double)SCREEN_SIZE;
	Topology::point_difference_type extent;
	extent[0] = extent[1] = (double)SCREEN_SIZE;
	rectangle_topology<> rect_top(gen,
		-SCREEN_SIZE / 2, -SCREEN_SIZE / 2,
		SCREEN_SIZE / 2, SCREEN_SIZE / 2);

	switch (LAYOUT_MODE) {
	case GRAPH_LAYOUT::RANDOM_LAYOUT:
		random_graph_layout(*graph, get(vertex_position, *graph), rect_top);
		break;

	case GRAPH_LAYOUT::CIRCLE_LAYOUT:
		circle_graph_layout(*graph, get(vertex_position, *graph), SCREEN_SIZE / 2);
		break;

	case GRAPH_LAYOUT::FRUCHTERMAN_REINGOLD_LAYOUT:
		fruchterman_reingold_force_directed_layout(*graph,
			get(vertex_position, *graph),
			topology,
			attractive_force(square_distance_attractive_force())
			.cooling(linear_cooling<double>(50))
		);
		break;
	}

	//add edges
	typedef square_topology<> Topology;
	typedef typename Topology::point_type Point;
	auto position = get(vertex_position, *graph);
	auto label = get(vertex_name, *graph);

	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typename graph_traits<Graph>::edge_iterator ei, ei_end;
	vertex_descriptor u, v;
	for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei) {
		u = source(*ei, *graph);
		v = target(*ei, *graph);
		Point p1 = position[u];
		Point p2 = position[v];

		//make edge item and push it to list
		EdgeItem *edge;
		edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::black), 0);

		edge->setPos(p1[0], p1[1]);
		edgeList << edge;
	}

	//add nodes
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi) {
		Point p = position[*vi];
		std::string name = label[*vi];

		//make node item and push it to list
		NodeItem *node;
		node = new NodeItem(p[0], p[1], QColor(Qt::green), QString(name.c_str()));

		node->setPos(QPointF(p[0], p[1]));
		nodeList << node;
	}

	return updateCoauthorList;
}

//override
QRectF CoauthorGraphItem::boundingRect() const
{
	//TODO
	return QRectF(-SCREEN_SIZE / 2, -SCREEN_SIZE / 2, SCREEN_SIZE, SCREEN_SIZE);
}

QPainterPath CoauthorGraphItem::shape() const
{
	QPainterPath path;
	return path;
}

void CoauthorGraphItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (!graph)
		return;

	//debug
	//-> x, y, w, h
	//현재 Graph의 bounding rect 출력
	/*QPen oldPen = painter->pen();
	QPen pen = oldPen;
	pen.setColor(Qt::red);
	painter->setPen(pen);
	painter->drawRect(QRectF(-SCREEN_SIZE/2, -SCREEN_SIZE/2, SCREEN_SIZE, SCREEN_SIZE));*/

	//print edges
	for (auto edge : edgeList) {
		edge->paint(painter, option, widget);
	}

	//print nodes
	for (auto node : nodeList) {
		node->paint(painter, option, widget);
	}
}

//event handler
void CoauthorGraphItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	qDebug() << "CoauthorGraphItem Click";
}

void CoauthorGraphItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
}

void CoauthorGraphItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}

//==================================================================================================
void CoauthorGraphItem::TopK(int K)
{
	for (int i = 0; i < K; i++)
	{
		if (i < K)
		{
			minHeap.push_back(nodeList[i]);
			_ReheapUp(minHeap, 0, i);
		}
		else
		{
			if (*minHeap[i] < *nodeList[i])
			{
				minHeap[0] = nodeList[i];
				_ReheapDown(minHeap, 0, K-1);
			}
		}
	}

	for (int i = 0; i < minHeap.size(); i++)
		minHeap[i]->setColor(QColor(Qt::red));
}

void CoauthorGraphItem::TopKfromA(int K, QString author)
{
	int source = -1;
	for(int i=0;i<nodeList.size();i++)
		if (author == nodeList[i]->getLabel())
		{
			source = i;
			break;
		}

	if (source == -1) {		// index를 못찾았을 시
		QMessageBox::information(nullptr, "잘못된 이름 입력", "author를 찾지 못했습니다.");
		return ;
	}

	vector<NodeItem*> adj;
	auto index = get(vertex_index, *graph);
	typename graph_traits<Graph>::edge_iterator ei, ei_end; int num;
	for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei)
	{
		num = index[ei->m_source];
		if (source == index[ei->m_target])
			adj.push_back(nodeList[num]);

		num = index[ei->m_target];
		if (source == index[ei->m_source])
			adj.push_back(nodeList[num]);
	}

	int size = K < adj.size() ? K : adj.size();

	for (int i = 0; i < adj.size(); i++)
	{
		if (i < size)
		{
			minHeap.push_back(adj[i]);
			_ReheapUp(minHeap, 0, i);
		}
		else
		{
			if (*minHeap[0] < *adj[i])
			{
				minHeap[0] = adj[i];
				_ReheapDown(minHeap, 0, K - 1);
			}
		}
	}

	for (int i = 0; i < minHeap.size(); i++)
		minHeap[i]->setColor(QColor(Qt::red));

	nodeList[source]->setColor(QColor(Qt::blue));
}



void CoauthorGraphItem::bfs(QString start, QString end) {
	int src, dest;
	for (int i = 0; i < nodeList.size(); i++)
		if (start == nodeList[i]->getLabel())
			src = i;
		else if (end == nodeList[i]->getLabel())
			dest = i;
	std::queue<int> q;
	std::queue<std::queue<int>> all_path;				// 전체 path를 인덱스로 나타냄
	bool* visited = new bool[nodeList.size()];
	int **link = new int*[nodeList.size()];	
	// 초기화	//////////////////////////////////////////////
	for (int i = 0; i < nodeList.size(); i++)
	{
		link[i] = new int[nodeList.size()];
		for (int j = 0; j < nodeList.size(); j++)
			link[i][j] = 0;
	}
	for (int cnt = 0; cnt < nodeList.size(); cnt++)
		visited[cnt] = false;
	//////////////////////////////////////////////////////////
	typedef property_map<Graph, vertex_index_t>::type vertex_index_map;
	vertex_index_map v_indexMap = get(vertex_index, *graph);
	graph_traits<Graph>::edge_iterator ei, ee;
	
	for (tie(ei, ee) = boost::edges(*graph); ei != ee; ++ei) {
		graph_traits<Graph>::edge_descriptor e = *ei;
		//auto a = get(edge_index, *ei,cgraph)
		graph_traits<Graph>::vertex_descriptor u = source(e, *graph), v = target(e, *graph);
		link[v_indexMap[u]][v_indexMap[v]] = 1;		// link[출발지][목적지] = 1;
		link[v_indexMap[v]][v_indexMap[u]] = 1;		// link[목적지][출발지] = 1;  because undirect graph
	}
	
	q.push(src);
	std::queue<int> tmp;
	tmp.push(src);
	all_path.push(tmp);
	visited[src] = true;
	
	int f, i;
	
	while (!q.empty())
	{
		f = q.front();
		q.pop();
		
		//qDebug() << f;
		
		for (i = 0; i < nodeList.size(); i++)
		{
			if (visited[i] == 0 && link[f][i] == 1)
			{
				std::queue<int> tmp = all_path.front();
				tmp.push(i);
				q.push(i);
				visited[i] = 1;
				all_path.push(tmp);
				if (i == dest)				// 목표지점에 도착했다면
				{
					int arrsize = tmp.size();
					int *arr = new int[arrsize];		// 최단거리 index 배열
					int i = 0;
					while (!tmp.empty())
					{
						qDebug() << tmp.front();
						arr[i] = tmp.front();
						tmp.pop();
						i++;
					}
					print_shortestPath(arr, arrsize);				// 최단거리 path 그리기
					return;
				}
			}
		}
	all_path.pop();
	}
	QMessageBox::warning(NULL, "There's no Connection", "There's no Connection", QMessageBox::Cancel, QMessageBox::Cancel);		// 길이 없을 때
}

void CoauthorGraphItem::print_shortestPath(int *arr, int arrSize)
{
	typedef property_map<Graph, vertex_index_t>::type vertex_index_map;
	typedef square_topology<> Topology;
	typedef typename Topology::point_type Point;
	auto position = get(vertex_position, *graph);
	auto verIndex = get(vertex_index, *graph);
	
	int arr_cnt = 0;
	bool exist_path = false;
	
	for (int i = 0; i < edgeList.size(); i++) {
		if (arr[arr_cnt] == edgeList[i]->getStartNodeIndex() && arr[arr_cnt + 1] == edgeList[i]->getEndNodeIndex() ||
			arr[arr_cnt] == edgeList[i]->getEndNodeIndex() && arr[arr_cnt + 1] == edgeList[i]->getStartNodeIndex()) {
			edgeList[i]->setColor(QColor(Qt::blue));
			edgeList[i]->setWidth(2);
			arr_cnt++;
			i = 0;
			exist_path = true;
		}
	}
	if (!exist_path)
		qDebug() << "There's no way to find it.";
	
}


void CoauthorGraphItem::chain(QString author1, QString author2)
{
	int src, dest;
	for (int i = 0; i < nodeList.size(); i++)
		if (author1 == nodeList[i]->getLabel())
			src = i;
		else if (author2 == nodeList[i]->getLabel())
			dest = i;
	//
	vector<int> parents;
	int** arr = new int*[nodeList.size()];
	for (int i = 0; i < nodeList.size(); i++)
	{
		arr[i] = new int[nodeList.size()];
		parents.push_back(i);
		for (int j = 0; j < nodeList.size(); j++)
			arr[i][j] = 0;
	}

	auto index = get(vertex_index, *graph);
	typename graph_traits<Graph>::edge_iterator ei, ei_end; int num1,num2;
	for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei)
	{
		num1 = index[ei->m_source];
		num2 = index[ei->m_target];
		
		arr[num1][num2] = arr[num2][num1] = 1;
	}

	bool* check = new bool[nodeList.size()];
	//
	vector<int> MaxHeap;
	
	MaxHeap.push_back(src);
	check[src] = true;
	bool find = false;

	while (!MaxHeap.empty()&&!find)
	{
		int temp = MaxHeap.front();
		MaxHeap.erase(MaxHeap.begin());
		ReheapDown(MaxHeap, 0, MaxHeap.size());

		for (int i = 0; i < nodeList.size(); i++)
		{
			if (arr[temp][i] == 1 && !check[i])
			{
				parents[i] = temp;
				check[i] = true;
				MaxHeap.push_back(i);
				ReheapUp(MaxHeap, 0, MaxHeap.size());

				if (i == dest)
					find = true;
			}
		}
	}
	//

	parents[src] = src;

	num1 = dest;
	num2 = parents[num1];

	auto position = get(vertex_position, *graph);

	typedef square_topology<> Topology;
	typedef typename Topology::point_type Point;
	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	vertex_descriptor u, v;  int i;
	for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei) {
		if ((index[ei->m_source] == num1 && index[ei->m_target] == num2) || (index[ei->m_source] == num2 && index[ei->m_target] == num1))
		{
			u = source(*ei, *graph);
			v = target(*ei, *graph);

			Point p1 = position[u];
			Point p2 = position[v];

			//make edge item and push it to list
			EdgeItem *edge;
			edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::blue), 3);

			edge->setPos(p1[0], p1[1]);
			edgeList << edge;

			num1 = num2;
			num2 = parents[num1];
			if (num1 == num2)
				break;
		}
	}
	edgeList[1]->setColor(QColor(Qt::blue));
	edgeList[2]->setColor(QColor(Qt::blue));
	nodeList[src]->setColor(QColor(Qt::blue));
	nodeList[dest]->setColor(QColor(Qt::blue));
}

//==================================================================================================
void CoauthorGraphItem::VisualizationInCircle()
{
	nodeList.clear();
	edgeList.clear();

	circle_graph_layout(*graph, get(vertex_position, *graph), SCREEN_SIZE / 2);

	//add edges
	typedef square_topology<> Topology;
	typedef typename Topology::point_type Point;
	auto position = get(vertex_position, *graph);
	auto label = get(vertex_name, *graph);

	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typename graph_traits<Graph>::edge_iterator ei, ei_end;
	vertex_descriptor u, v;
	for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei) {
		u = source(*ei, *graph);
		v = target(*ei, *graph);
		Point p1 = position[u];
		Point p2 = position[v];

		//make edge item and push it to list
		EdgeItem *edge;
		edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::black), 0);

		edge->setPos(0, 0);
		edgeList << edge;
	}

	//add nodes
	typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
	vertex_iterator vi, vi_end;
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi) {
		Point p = position[*vi];
		std::string name = label[*vi];

		//make node item and push it to list
		NodeItem *node;
		node = new NodeItem(p[0], p[1], QColor(Qt::green), QString(name.c_str()));

		node->setPos(QPointF(p[0], p[1]));
		nodeList << node;
	}
}

void CoauthorGraphItem::paint_percent(QString author)
{

	int idx;
	int cnt = 0;


	for (int i = 0; i < nodeList.size(); i++)
	{
		if (author == nodeList[i]->getLabel())
		{
			idx = i;
			nodeList[i]->setColor(QColor(Qt::blue));
		}
	}

	for (int i = 0; i < edgeList.size(); i++)
	{
		if (edgeList[i]->getEndNodeIndex() == idx || edgeList[i]->getStartNodeIndex() == idx)
		{
			cnt++;
		}
	}
	cnt = 100 / cnt;
	QString k;
	string k2;

	k2 = to_string(cnt);
	k2 += '%';
	k = QString::fromStdString(k2);

	if (!graph)
	{
		return;
	}

	for (auto edge : edgeList)
	{
		if (edge->getEndNodeIndex() == idx || edge->getStartNodeIndex() == idx)
		{
			edge->setLabel(k);
		}
		else
		{
		}
	}


}

//==================================================================================================

PaperGraphItem::PaperGraphItem(ifstream& fin)
{
	if (!fin)
		throw std::exception("paper graph file input is invalid");

	/**
	*	Parse Paper dataset
	*	- paper_key, [author_list], publish_year
	*	Column Delimiter:		||
	*	Author list Delimiter:	&&
	*/
	std::string line;
	vector<std::string> tokens;
	vector<std::string> authors;

	node_cnt = 0;
	qDebug() << "* paper graph reading start";

	//한 줄씩 읽어서 Parse
	while (std::getline(fin, line) && !line.empty()) {
		line_cnt++;

		//boost::split 이용해 문자열 분리
		//tokens[0]: Paper-key.	ex) conf/iastedCSN/KeimS06
		//tokens[1]: Authors.	ex) Werner Keim&&Arpad L. Scholtz
		//tokens[2]: Published year.
		boost::split(tokens, line, boost::is_any_of("||"), boost::token_compress_on);
		boost::split(authors, tokens[1], boost::is_any_of("&&"), boost::token_compress_on);
		
		string& paper_key = tokens[0];
		if (tokens.size() > 2) {
			paper_key += string("||") + tokens[2];
		}
		if (node_ids.left.find(paper_key) == node_ids.left.end()) {
			node_ids.insert(bm_type::value_type(paper_key, node_cnt++));

		}

		for (auto author : authors) {
			edges.push_back(pair<string, string>(paper_key, author));
			if (node_ids.left.find(author) == node_ids.left.end()) {
				node_ids.insert(bm_type::value_type(author, node_cnt++));
			}
		}

		//debug
		if (node_cnt > NODE_LIMIT) break;
	}
	qDebug() << "* paper graph reading complete";
	qDebug() << "* # of nodes: " << node_cnt;
	qDebug() << "* # of edges: " << edges.size();

	//edge conversion
	//<string, string> to <int, int>
	//using boost::bimap (bidirectional map)
	for (auto edge : edges) {
		edges_indexes.push_back({
			node_ids.left.find(edge.first)->get_right(),
			node_ids.left.find(edge.second)->get_right()
		});
	}
	//Graph --> defined in "PaperGraphWidget.h"
	//Graph graph(edges_indexes.begin(), edges_indexes.end(), node_ids.size());
	graph = new Graph(edges_indexes.begin(), edges_indexes.end(), node_ids.size());

	//set index property
	qDebug() << "* set vertex property start";
	typedef typename graph_traits<Graph>::edge_iterator edge_iterator;
	typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
	vertex_iterator vi, vi_end;
	int i = 0;
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi) {
		//Vertex Property 설정
		//index: 0 ~ ...
		//name : map의 value(i) 기준으로 찾은 Key
		//		map --> map<string, int> (boost bidirectional map)
		boost::put(vertex_index, *graph, *vi, i);
		boost::put(vertex_name, *graph, *vi,
			node_ids.right.find(i)->get_left());

		++i;
	}
	qDebug() << "* set vertex property end";

	qDebug() << "* make graph layout start";
	typedef square_topology<> Topology;
	boost::minstd_rand gen;
	Topology topology(gen, (double)SCREEN_SIZE);
	Topology::point_type origin;
	origin[0] = origin[1] = (double)SCREEN_SIZE;
	Topology::point_difference_type extent;
	extent[0] = extent[1] = (double)SCREEN_SIZE;
	rectangle_topology<> rect_top(gen,
		-SCREEN_SIZE / 2, -SCREEN_SIZE / 2,
		SCREEN_SIZE / 2, SCREEN_SIZE / 2);

	switch (LAYOUT_MODE) {
	case GRAPH_LAYOUT::RANDOM_LAYOUT:
		random_graph_layout(*graph, get(vertex_position, *graph), rect_top);
		break;

	case GRAPH_LAYOUT::CIRCLE_LAYOUT:
		circle_graph_layout(*graph, get(vertex_position, *graph), SCREEN_SIZE / 2);
		break;

	case GRAPH_LAYOUT::FRUCHTERMAN_REINGOLD_LAYOUT:
		fruchterman_reingold_force_directed_layout(*graph,
			get(vertex_position, *graph),
			topology,
			attractive_force(square_distance_attractive_force())
			.cooling(linear_cooling<double>(50))
		);
		break;
	}
	qDebug() << "* make graph layout end";


	//add edges
	typedef square_topology<> Topology;
	typedef typename Topology::point_type Point;
	auto position = get(vertex_position, *graph);
	auto label = get(vertex_name, *graph);

	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typename graph_traits<Graph>::edge_iterator ei, ei_end;
	vertex_descriptor u, v;
	for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei) {
		u = source(*ei, *graph);
		v = target(*ei, *graph);
		Point p1 = position[u];
		Point p2 = position[v];

		//make edge item and push it to list
		EdgeItem *edge;

		if (label[u] == "conf/sbrn/GomesPSRC10" ||
			label[u] == "conf/iastedCSN/KeimS06" ||
			label[v] == "conf/sbrn/GomesPSRC10" ||
			label[v] == "conf/iastedCSN/KeimS06") {
			//highlight
			edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::blue), 3);
		}
		else {
			edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::black), 0);
		}
		edge->setPos(p1[0], p1[1]);
		edgeList << edge;
	}

	//add nodes
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi) {
		Point p = position[*vi];
		std::string name = label[*vi];

		//make node item and push it to list
		NodeItem *node;
		if (name == "conf/sbrn/GomesPSRC10" ||
			name == "conf/iastedCSN/KeimS06") {
			//highlight
			node = new NodeItem(p[0], p[1], QColor(Qt::blue), QString(name.c_str()));
		}
		else {
			node = new NodeItem(p[0], p[1], QColor(Qt::green), QString(name.c_str()));
		}
		node->setPos(QPointF(p[0], p[1]));
		nodeList << node;
	}
}

PaperGraphItem::PaperGraphItem(Graph* _graph)
{
	graph = _graph;

	typedef typename graph_traits<Graph>::edge_iterator edge_iterator;
	typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
	vertex_iterator vi, vi_end;

	typedef square_topology<> Topology;
	boost::minstd_rand gen;
	Topology topology(gen, (double)SCREEN_SIZE);
	Topology::point_type origin;
	origin[0] = origin[1] = (double)SCREEN_SIZE;
	Topology::point_difference_type extent;
	extent[0] = extent[1] = (double)SCREEN_SIZE;
	rectangle_topology<> rect_top(gen,
		-SCREEN_SIZE / 2, -SCREEN_SIZE / 2,
		SCREEN_SIZE / 2, SCREEN_SIZE / 2);

	switch (LAYOUT_MODE) {
	case GRAPH_LAYOUT::RANDOM_LAYOUT:
		random_graph_layout(*graph, get(vertex_position, *graph), rect_top);
		break;

	case GRAPH_LAYOUT::CIRCLE_LAYOUT:
		circle_graph_layout(*graph, get(vertex_position, *graph), SCREEN_SIZE / 2);
		break;

	case GRAPH_LAYOUT::FRUCHTERMAN_REINGOLD_LAYOUT:
		fruchterman_reingold_force_directed_layout(*graph,
			get(vertex_position, *graph),
			topology,
			attractive_force(square_distance_attractive_force())
			.cooling(linear_cooling<double>(50))
		);
		break;
	}
	// vertex info(참여도) initialization
	vector<int> vInfo;
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi)
		vInfo.push_back(0);

	//add edges
	typedef square_topology<> Topology;
	typedef typename Topology::point_type Point;
	auto position = get(vertex_position, *graph);
	auto index = get(vertex_index, *graph);
	auto label = get(vertex_name, *graph);

	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typename graph_traits<Graph>::edge_iterator ei, ei_end;
	vertex_descriptor u, v; int i;
	for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei) {
		// vertex info set
		i = index[ei->m_source];
		vInfo[i]++;
		i = index[ei->m_target];
		vInfo[i]++;

		u = source(*ei, *graph);
		v = target(*ei, *graph);

		Point p1 = position[u];
		Point p2 = position[v];

		//make edge item and push it to list
		EdgeItem *edge;

		/*   if (label[u] == "conf/sbrn/GomesPSRC10" ||
		label[u] == "conf/iastedCSN/KeimS06" ||
		label[v] == "conf/sbrn/GomesPSRC10" ||
		label[v] == "conf/iastedCSN/KeimS06") {
		//highlight
		edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::blue), 3);
		}
		else {
		edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::black), 0);
		}*/
		edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::black), 0);
		edge->setPos(p1[0], p1[1]);
		edgeList << edge;
	}

	//add nodes
	i = 0;
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi) {
		Point p = position[*vi];
		std::string name = label[*vi];

		//make node item and push it to list
		NodeItem *node;
		/*if (name == "conf/sbrn/GomesPSRC10" ||
		name == "conf/iastedCSN/KeimS06") {
		//highlight
		node = new NodeItem(p[0], p[1], QColor(Qt::blue), QString(name.c_str()));
		}
		else {
		node = new NodeItem(p[0], p[1], QColor(Qt::green), QString(name.c_str()));
		}*/

		node = new NodeItem(p[0], p[1], QColor(Qt::green), QString(name.c_str()), vInfo[i++]);
		node->setPos(QPointF(p[0], p[1]));
		nodeList << node;
	}
}

void PaperGraphItem::updateGraph(ifstream& fin)
{
	if (!fin)
		throw std::exception("paper graph file input is invalid");

	/**
	*	Parse Paper dataset
	*	- paper_key, [author_list], publish_year
	*	Column Delimiter:		||
	*	Author list Delimiter:	&&
	*/
	std::string line;
	vector<std::string> tokens;
	vector<std::string> authors;

	qDebug() << "* paper graph update start";

	int local_line_cnt = 0;

	//한 줄씩 읽어서 Parse
	while (std::getline(fin, line) && !line.empty()) {

		local_line_cnt++;

		if (local_line_cnt > line_cnt) {

			line_cnt++;

			//boost::split 이용해 문자열 분리
			//tokens[0]: Paper-key.	ex) conf/iastedCSN/KeimS06
			//tokens[1]: Authors.	ex) Werner Keim&&Arpad L. Scholtz
			//tokens[2]: Published year.
			boost::split(tokens, line, boost::is_any_of("||"), boost::token_compress_on);
			boost::split(authors, tokens[1], boost::is_any_of("&&"), boost::token_compress_on);

			const string& paper_key = tokens[0];
			if (node_ids.left.find(paper_key) == node_ids.left.end()) {
				node_ids.insert(bm_type::value_type(paper_key, node_cnt++));
			}

			for (auto author : authors) {
				edges.push_back(pair<string, string>(paper_key, author));
				if (node_ids.left.find(author) == node_ids.left.end()) {
					node_ids.insert(bm_type::value_type(author, node_cnt++));
				}
			}
		}

		//debug
		if (node_cnt > NODE_LIMIT) break;

	}
	qDebug() << "* paper graph update complete";
	qDebug() << "* # of nodes: " << node_cnt;
	qDebug() << "* # of edges: " << edges.size();

	//edge conversion
	//<string, string> to <int, int>
	//using boost::bimap (bidirectional map)
	for (auto edge : edges) {
		edges_indexes.push_back({
			node_ids.left.find(edge.first)->get_right(),
			node_ids.left.find(edge.second)->get_right()
		});
	}
	//Graph --> defined in "PaperGraphWidget.h"
	//Graph graph(edges_indexes.begin(), edges_indexes.end(), node_ids.size());
	graph = new Graph(edges_indexes.begin(), edges_indexes.end(), node_ids.size());

	typedef typename graph_traits<Graph>::edge_iterator edge_iterator;
	typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
	vertex_iterator vi, vi_end;
	int i = 0;
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi) {
		//Vertex Property 설정
		//index: 0 ~ ...
		//name : map의 value(i) 기준으로 찾은 Key
		//		map --> map<string, int> (boost bidirectional map)
		boost::put(vertex_index, *graph, *vi, i);
		boost::put(vertex_name, *graph, *vi,
			node_ids.right.find(i)->get_left());

		++i;
	}

	typedef square_topology<> Topology;
	boost::minstd_rand gen;
	Topology topology(gen, (double)SCREEN_SIZE);
	Topology::point_type origin;
	origin[0] = origin[1] = (double)SCREEN_SIZE;
	Topology::point_difference_type extent;
	extent[0] = extent[1] = (double)SCREEN_SIZE;
	rectangle_topology<> rect_top(gen,
		-SCREEN_SIZE / 2, -SCREEN_SIZE / 2,
		SCREEN_SIZE / 2, SCREEN_SIZE / 2);

	switch (LAYOUT_MODE) {
	case GRAPH_LAYOUT::RANDOM_LAYOUT:
		random_graph_layout(*graph, get(vertex_position, *graph), rect_top);
		break;

	case GRAPH_LAYOUT::CIRCLE_LAYOUT:
		circle_graph_layout(*graph, get(vertex_position, *graph), SCREEN_SIZE / 2);
		break;

	case GRAPH_LAYOUT::FRUCHTERMAN_REINGOLD_LAYOUT:
		fruchterman_reingold_force_directed_layout(*graph,
			get(vertex_position, *graph),
			topology,
			attractive_force(square_distance_attractive_force())
			.cooling(linear_cooling<double>(50))
		);
		break;
	}

	//add edges
	typedef square_topology<> Topology;
	typedef typename Topology::point_type Point;
	auto position = get(vertex_position, *graph);
	auto label = get(vertex_name, *graph);

	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typename graph_traits<Graph>::edge_iterator ei, ei_end;
	vertex_descriptor u, v;
	for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei) {
		u = source(*ei, *graph);
		v = target(*ei, *graph);
		Point p1 = position[u];
		Point p2 = position[v];

		//make edge item and push it to list
		EdgeItem *edge;

		if (label[u] == "conf/sbrn/GomesPSRC10" ||
			label[u] == "conf/iastedCSN/KeimS06" ||
			label[v] == "conf/sbrn/GomesPSRC10" ||
			label[v] == "conf/iastedCSN/KeimS06") {
			//highlight
			edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::blue), 3);
		}
		else {
			edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::black), 0);
		}
		edge->setPos(p1[0], p1[1]);
		edgeList << edge;
	}

	//add nodes
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi) {
		Point p = position[*vi];
		std::string name = label[*vi];

		//make node item and push it to list
		NodeItem *node;
		if (name == "conf/sbrn/GomesPSRC10" ||
			name == "conf/iastedCSN/KeimS06") {
			//highlight
			node = new NodeItem(p[0], p[1], QColor(Qt::blue), QString(name.c_str()));
		}
		else {
			node = new NodeItem(p[0], p[1], QColor(Qt::green), QString(name.c_str()));
		}
		node->setPos(QPointF(p[0], p[1]));
		nodeList << node;
	}
}

//override
QRectF PaperGraphItem::boundingRect() const
{
	//TODO
	return QRectF(-SCREEN_SIZE / 2, -SCREEN_SIZE / 2, SCREEN_SIZE, SCREEN_SIZE);
}

QPainterPath PaperGraphItem::shape() const
{
	QPainterPath path;
	return path;
}

void PaperGraphItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (!graph)
		return;

	//debug
	//-> x, y, w, h
	//현재 Graph의 bounding rect 출력
	/*QPen oldPen = painter->pen();
	QPen pen = oldPen;
	pen.setColor(Qt::red);
	painter->setPen(pen);
	painter->drawRect(QRectF(-SCREEN_SIZE/2, -SCREEN_SIZE/2, SCREEN_SIZE, SCREEN_SIZE));*/

	//print edges
	for (auto edge : edgeList) {
		edge->paint(painter, option, widget);
	}

	//print nodes
	for (auto node : nodeList) {
		node->paint(painter, option, widget);
	}
}

//event handler
void PaperGraphItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void PaperGraphItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
}

void PaperGraphItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}

void PaperGraphItem::Filtering(QString _year, QString _conf)
{
	//_year.remove(0, 2);
	string year = _year.toUtf8().constData();
	string conf = _conf.toUtf8().constData();

	nodeList.clear();
	edgeList.clear();

	typedef typename graph_traits<Graph>::edge_iterator edge_iterator;
	typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
	vertex_iterator vi, vi_end;

	typedef square_topology<> Topology;
	typedef typename Topology::point_type Point;
	auto position = get(vertex_position, *graph);
	auto label = get(vertex_name, *graph);

	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typename graph_traits<Graph>::edge_iterator ei, ei_end;
	vertex_descriptor u, v;
	for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei) {
		u = source(*ei, *graph);
		v = target(*ei, *graph);
		Point p1 = position[u];
		Point p2 = position[v];

		EdgeItem *edge;
		NodeItem *node1;
		NodeItem *node2;
		qDebug() << "label[u]: " << label[u].c_str() << " label[v]: " << label[v].c_str() << endl;
		if ((label[u].find(conf) != -1 || label[v].find(conf) != -1) && (label[u].find(year) != -1 || label[v].find(year) != -1))
		{
			edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::black), 0);
			edge->setPos(QPoint(0, 0));
			edgeList << edge;

			node1 = new NodeItem(p1[0], p1[1], QColor(Qt::green), QString(label[u].c_str()));
			node2 = new NodeItem(p2[0], p2[1], QColor(Qt::green), QString(label[v].c_str()));

			node1->setPos(QPoint(0, 0));
			node2->setPos(QPoint(0, 0));
			nodeList << node1 << node2;
		}
	}

	for (int i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->getLabel().contains("conf"))
			nodeList[i]->setColor(QColor(Qt::blue));
	}
}
void PaperGraphItem::TopKUsingPaper(QString criteria, QString _year, QString _conf, int K)
{
	_year.remove(0, 2);
	string year = _year.toUtf8().constData();
	string conf = _conf.toUtf8().constData();

	typedef typename graph_traits<Graph>::edge_iterator edge_iterator;
	typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
	vertex_iterator vi, vi_end;
	typedef square_topology<> Topology;
	typedef typename Topology::point_type Point;
	auto position = get(vertex_position, *graph);
	auto index = get(vertex_index, *graph);
	auto label = get(vertex_name, *graph);

	typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
	typename graph_traits<Graph>::edge_iterator ei, ei_end;
	vertex_descriptor u, v;

	vector<int> vInfo;
	for (boost::tie(vi, vi_end) = vertices(*graph); vi != vi_end; ++vi)
		vInfo.push_back(0);

	int num;
	for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei) {
		// vertex info set
		num = index[ei->m_source];
		vInfo[num]++;
		num = index[ei->m_target];
		vInfo[num]++;
	}

	if (criteria == "Year")
	{
		nodeList.clear();
		edgeList.clear();

		for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei) {
			u = source(*ei, *graph);
			v = target(*ei, *graph);
			Point p1 = position[u];
			Point p2 = position[v];

			int num1 = index[ei->m_source];
			int num2 = index[ei->m_target];

			//make edge item and push it to list
			EdgeItem *edge;
			NodeItem *node1;
			NodeItem *node2;
			if (label[u].find(year) != -1 || label[v].find(year) != -1)
			{
				edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::black), 0);
				edge->setPos(p1[0], p1[1]);
				edgeList << edge;

				node1 = new NodeItem(p1[0], p1[1], QColor(Qt::green), QString(label[u].c_str()), vInfo[num1]);
				node2 = new NodeItem(p2[0], p2[1], QColor(Qt::green), QString(label[v].c_str()), vInfo[num2]);

				node1->setPos(QPoint(p1[0], p1[1]));
				node2->setPos(QPoint(p2[0], p2[1]));
				nodeList << node1 << node2;
			}
		}
	}
	else if (criteria == "Conference")
	{
		nodeList.clear();
		edgeList.clear();

		for (boost::tie(ei, ei_end) = boost::edges(*graph); ei != ei_end; ++ei) {
			u = source(*ei, *graph);
			v = target(*ei, *graph);
			Point p1 = position[u];
			Point p2 = position[v];

			int num1 = index[ei->m_source];
			int num2 = index[ei->m_target];

			//make edge item and push it to list
			EdgeItem *edge;
			NodeItem *node1;
			NodeItem *node2;
			if (label[u].find(conf) != -1 || label[v].find(conf) != -1)
			{
				edge = new EdgeItem(p1[0], p1[1], p2[0], p2[1], QColor(Qt::black), 0);
				edge->setPos(p1[0], p1[1]);
				edgeList << edge;

				node1 = new NodeItem(p1[0], p1[1], QColor(Qt::green), QString(label[u].c_str()), vInfo[num1]);
				node2 = new NodeItem(p2[0], p2[1], QColor(Qt::green), QString(label[v].c_str()), vInfo[num2]);

				node1->setPos(QPoint(p1[0], p1[1]));
				node2->setPos(QPoint(p2[0], p2[1]));
				nodeList << node1 << node2;
			}
		}
	}

	qDebug() << "hello";

	int j = 0;
	for (int i = 0; i < K; i++)
	{
		if (nodeList[i]->getLabel().contains("conf"))
			continue;

		if (j < K)
		{
			j++;
			minHeap.push_back(nodeList[i]);
			_ReheapUp(minHeap, 0, j);
		}
		else
		{
			if (*minHeap[0] < *nodeList[i])
			{
				minHeap[0] = nodeList[i];
				_ReheapDown(minHeap, 0, K - 1);
			}
		}
	}

	for (int i = 0; i < minHeap.size(); i++)
		minHeap[i]->setColor(QColor(Qt::red));
}