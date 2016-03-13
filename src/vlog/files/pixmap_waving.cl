__kernel void pixmapWaving(__global uchar* input,
						   __global uchar* output,
						   int width,
						   int height,
						   __global int* coords)
{
	int x = get_global_id(0);
	int y = get_global_id(1);

	if (x >= width || y >= height)
	{
		return;
	}

	int2 pnt = width > height ? (int2)(x, y + coords[x]) : (int2)(x + coords[y], y);

	if ((width >= height) && (pnt.y >= height || pnt.y < 0))
	{
		return;
	}

	if ((width < height) && (pnt.x >= width || pnt.x < 0))
	{
		return;
	}

	output[width * 4 * pnt.y + pnt.x * 4] = input[width * 4 * y + x * 4];
	output[width * 4 * pnt.y + pnt.x * 4 + 1] = input[width * 4 * y + x * 4 + 1];
	output[width * 4 * pnt.y + pnt.x * 4 + 2] = input[width * 4 * y + x * 4 + 2];
	output[width * 4 * pnt.y + pnt.x * 4 + 3] = input[width * 4 * y + x * 4 + 3];
}
