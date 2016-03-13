#pragma once

#include "../config.h"

#ifdef VL_USE_OPENCL

#include "../system/log.h"

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QImage>

#include <cl/cl.h>

namespace General
{
	class COpenCLFramework
	{
	public:
		enum ETargetDevice
		{
			TD_CPU,
			TD_GPU
		};

		COpenCLFramework();
		~COpenCLFramework();

		bool initialize(ETargetDevice target, unsigned char* source, unsigned char* dest, int width, int height);
		void destroy();

		bool execute(int* coords);

	private:
		cl_platform_id m_plaform;
		cl_device_id m_device;
		cl_context m_context;
		cl_command_queue m_queue;
		cl_program m_program;
		cl_kernel m_kernel;

		size_t m_kernel_work_group_size;

		size_t m_global_work_size[2];
		size_t m_local_work_size[2];

		size_t m_buffer_size;

		cl_mem m_input_mem;
		cl_mem m_output_mem;

		int m_image_width;
		int m_image_height;

		unsigned char *m_input_image;
		unsigned char *m_output_image;

	};
}

#endif // VL_USE_OPENCL
