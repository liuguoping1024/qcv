#include <stdlib.h>
#include <math.h>
#include <common/ImageSource.h>
#include <common/ImageSourceIO.h>
#include <segment/ImageSegment.h>
#include <exception/IllegalArgumentException.h>

#include <algorithm>
#include <cmath>

namespace qcv {

template <class T>
inline T square(const T &x) { return x*x; };


universe::universe(int elements) {
	elts = new uni_elt[elements];
	num = elements;
	for (int i = 0; i < elements; i++) {
		elts[i].rank = 0;
		elts[i].size = 1;
		elts[i].p = i;
	}
}

universe::~universe() {
	delete[] elts;
}

int universe::find(int x) {
	int y = x;
	while (y != elts[y].p)
		y = elts[y].p;
	elts[x].p = y;
	return y;
}

void universe::join(int x, int y) {
	if (elts[x].rank > elts[y].rank) {
		elts[y].p = x;
		elts[x].size += elts[y].size;
	} else {
		elts[x].p = y;
		elts[y].size += elts[x].size;
		if (elts[x].rank == elts[y].rank)
			elts[y].rank++;
	}
	num--;
}


typedef struct {
  float w;
  int a, b;
} edge;

bool operator<(const edge &a, const edge &b) {
  return a.w < b.w;
}

#define THRESHOLD(size, c) (c/size)

/*
 * Segment a graph
 *
 * Returns a disjoint-set forest representing the segmentation.
 *
 * num_vertices: number of vertices in graph.
 * num_edges: number of edges in graph
 * edges: array of edges.
 * c: constant for treshold function.
 */
static universe *segment_graph(int num_vertices, int num_edges, edge *edges, float c) {
	// sort edges by weight
	std::sort(edges, edges + num_edges);

	// make a disjoint-set forest
	universe *u = new universe(num_vertices);

	// init thresholds
	float *threshold = new float[num_vertices];
	for (int i = 0; i < num_vertices; i++)
	{
		threshold[i] = THRESHOLD(1, c);
	}

	// for each edge, in non-decreasing weight order...
	for (int i = 0; i < num_edges; i++) {
		edge *pedge = &edges[i];

		// components conected by this edge
		int a = u->find(pedge->a);
		int b = u->find(pedge->b);
		if (a != b) {
			if ((pedge->w <= threshold[a]) && (pedge->w <= threshold[b])) {
				u->join(a, b);
				a = u->find(a);
				threshold[a] = pedge->w + THRESHOLD(u->size(a), c);
			}
		}
	}

	// free up
	delete threshold;
	return u;
}


// random color
static rgb random_rgb(){
  rgb c;
  //double r;

  c.r = (uint8_t)random();
  c.g = (uint8_t)random();
  c.b = (uint8_t)random();

  return c;
}

// dissimilarity measure between pixels
static inline float diff(ImageSource<float> *r, ImageSource<float> *g, ImageSource<float> *b,
		int x1, int y1, int x2, int y2) {
	return sqrt(square(r->imRef(x1, y1) - r->imRef(x2, y2))
					+ square( g->imRef(x1, y1) - g->imRef(x2, y2))
					+ square( b->imRef(x1, y1) - b->imRef(x2, y2)));
}

/*
 * Segment an image
 *
 * Returns a color image representing the segmentation.
 *
 * im: image to segment.
 * sigma: to smooth the image.
 * c: constant for treshold function.
 * min_size: minimum component size (enforced by post-processing stage).
 * num_ccs: number of connected components in the segmentation.
 */
ImageSource<rgb> * segment_image(ImageSource<qcv::rgb> *im, float sigma, float c, int min_size, int *num_ccs) {
	int width = im->width();
	int height = im->height();

	ImageSource<uint8_t> *r = new ImageSource<uint8_t>(width, height);
	ImageSource<uint8_t> *g = new ImageSource<uint8_t>(width, height);
	ImageSource<uint8_t> *b = new ImageSource<uint8_t>(width, height);

	// smooth each color channel
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int offset = x + y * width;
			r->imRef(offset) = im->imRef(offset).r;
			g->imRef(offset) = im->imRef(offset).g;
			b->imRef(offset) = im->imRef(offset).b;
		}
	}

	ImageSource<float> *smooth_r = smooth(r, sigma);
	ImageSource<float> *smooth_g = smooth(g, sigma);
	ImageSource<float> *smooth_b = smooth(b, sigma);

	delete r;
	delete g;
	delete b;

	// build graph
	edge *edges = new edge[width * height * 4];
	int num = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (x < width - 1) {
				edges[num].a = y * width + x;
				edges[num].b = y * width + (x + 1);
				edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x + 1, y);
				num++;
			}

			if (y < height - 1) {
				edges[num].a = y * width + x;
				edges[num].b = (y + 1) * width + x;
				edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x,
						y + 1);
				num++;
			}

			if ((x < width - 1) && (y < height - 1)) {
				edges[num].a = y * width + x;
				edges[num].b = (y + 1) * width + (x + 1);
				edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x + 1,
						y + 1);
				num++;
			}

			if ((x < width - 1) && (y > 0)) {
				edges[num].a = y * width + x;
				edges[num].b = (y - 1) * width + (x + 1);
				edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x + 1,
						y - 1);
				num++;
			}
		}
	}
	delete smooth_r;
	delete smooth_g;
	delete smooth_b;

	// segment
	universe *u = segment_graph(width * height, num, edges, c);

	// post process small components
	for (int i = 0; i < num; i++) {
		int a = u->find(edges[i].a);
		int b = u->find(edges[i].b);
		if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
			u->join(a, b);
	}
	delete[] edges;
	*num_ccs = u->num_sets();

	ImageSource<rgb> *output = new ImageSource<rgb>(width, height);

	// pick random colors for each component
	rgb *colors = new rgb[width * height];
	for (int i = 0; i < width * height; i++)
		colors[i] = random_rgb();

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int comp = u->find(y * width + x);
			output->imRef(x, y) = colors[comp];
		}
	}

	delete[] colors;
	delete u;

	return output;
}

}
