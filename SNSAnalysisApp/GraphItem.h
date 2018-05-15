#pragma once

#include "NodeItem.h"
#include "EdgeItem.h"

#include <QGraphicsItem>
#include <QList>
#include <QColor>
#include <QDebug>
#include <QtWidgets>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/fruchterman_reingold.hpp>
#include <boost/graph/random_layout.hpp>
#include <boost/graph/circle_layout.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/algorithm/string.hpp>	//boost::split
#include <boost/bimap.hpp>
#include <boost/regex.hpp>
#include <boost/graph/copy.hpp>

#include <exception>
#include <iterator>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <queue>

using namespace std;
using namespace boost;


enum GRAPH_LAYOUT {
	RANDOM_LAYOUT,
	CIRCLE_LAYOUT,
	//KAMADA_KAWAI_LAYOUT,
	FRUCHTERMAN_REINGOLD_LAYOUT	//slow
};

const int LAYOUT_MODE = GRAPH_LAYOUT::RANDOM_LAYOUT;
const int SCREEN_SIZE = 1000;
const int NODE_LIMIT = 1000;

enum vertex_position_t { vertex_position };
namespace boost {
	BOOST_INSTALL_PROPERTY(vertex, position);
}
typedef square_topology<>::point_type point;
struct simple_edge {
	int first, second;
};
typedef boost::property<vertex_index_t, int,
	boost::property<vertex_name_t, std::string,
	boost::property<vertex_position_t, point>>
	> VertexProperties;
typedef adjacency_list<
	listS,	//outEdgeList
	listS,	//VertexList
	undirectedS,
	//vertex properties
	VertexProperties,
	//edge properties
	boost::property<edge_weight_t, double>
> Graph;

typedef boost::bimap<string, int> bm_type;

//==================================================================================================

class GraphItem {
public: 
	GraphItem() {};
	GraphItem(vector<pair<string, string>> edges, bm_type node_ids, 
		vector<simple_edge> edges_indexes, int node_cnt, int line_cnt) {
		this->edges = edges;
		this->node_ids = node_ids;
		this->edges_indexes = edges_indexes;
		this->node_cnt = node_cnt;
		this->line_cnt = line_cnt;
	};
	~GraphItem() {};

	// get set
	QList<NodeItem *> getNodeList() { return nodeList; }
	QList<EdgeItem *> getEdgeList() { return edgeList; }

	int getFileSize() {
		return curFileSize;
	}

	void setFileSize(int size) {
		curFileSize = size;
	}

protected:
	Graph * graph = nullptr;
	QList<NodeItem *> nodeList;
	QList<EdgeItem *> edgeList;
	vector<NodeItem*> minHeap;

	int curFileSize;				// 최근 파일 크기. 파일 변경확인

	vector<pair<string, string>> edges;
	bm_type node_ids;
	vector<simple_edge> edges_indexes;	//int로 변환된 edge

	int node_cnt = 0;
	int line_cnt = 0;
};

class CoauthorGraphItem
	: public QGraphicsItem, public GraphItem
{
public:
	CoauthorGraphItem(ifstream& fin);
	CoauthorGraphItem(CoauthorGraphItem& graph);
	QStringList* updateGraph(ifstream& fin);

	//overrides
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	//methods
	void path_highlighting(std::string start, std::string end);
	Graph* getGraph();
	void TopK(int K);
	void TopKfromA(int K, QString author);
	void chain(QString author1, QString author2);
	void bfs(QString start, QString end);
	void print_shortestPath(int *arr, int arrSize);
	void paint_percent(QString author);

	void VisualizationInCircle();

	void ReheapUp(vector<int> v, int root, int last);
	void ReheapDown(vector<int> v, int root, int last);
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

//==================================================================================================

class PaperGraphItem
	: public QGraphicsItem, public GraphItem
{
public:
	PaperGraphItem(ifstream& fin);
	PaperGraphItem(Graph* graph);		// 생김
	void updateGraph(ifstream& fin);

	//overrides
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	//methods
	void path_highlighting(std::string start, std::string end);
	Graph* getGraph();
	void Filtering(QString year, QString conf);
	void TopKUsingPaper(QString criteria, QString year, QString conf, int K);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};