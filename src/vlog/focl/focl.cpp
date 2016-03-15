/****************************************************************************
 * Copyright (C) 2012-2013 Ievgen Polyvanyi
 * Copyright (C) 2012-2016 Oleg Gushchenkov
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
****************************************************************************/

#include "focl.h"

#ifdef VL_USE_OPENCL

#include <QDebug>

namespace General
{
	COpenCLFramework::COpenCLFramework()
		: m_program(nullptr), m_kernel(nullptr), m_context(nullptr), m_queue(nullptr),
		  m_input_image(nullptr), m_output_image(nullptr), m_input_mem(nullptr), m_output_mem(nullptr)
	{
	}

	COpenCLFramework::~COpenCLFramework()
	{
		destroy();
	}

	bool COpenCLFramework::initialize(ETargetDevice target, unsigned char* source, unsigned char* dest, int width, int height)
	{
		cl_device_type device_type = (target == TD_GPU ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU);

		CLog::getInstance()->information(QString(QObject::tr("Initialization OpenCL framework (on ")) + QString(device_type == CL_DEVICE_TYPE_GPU ? "GPU" : "CPU") + QString(").."));

		cl_int error;

		error = clGetPlatformIDs(1, &m_plaform, NULL);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't get OpenCL platform, error code: ")) + QString::number(error));

			return false;
		}

		error = clGetDeviceIDs(m_plaform, device_type, 1, &m_device, NULL);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->warning(QString(QObject::tr("Can't get OpenCL device (") + QString(device_type == CL_DEVICE_TYPE_GPU ? "GPU" : "CPU") + QString(")") + QObject::tr(", error code: ")) + QString::number(error));

			return false;
		}

		m_context = clCreateContext(NULL, 1, &m_device, NULL, NULL, &error);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't create OpenCL context, error code: ")) + QString::number(error));

			return false;
		}

		m_queue = clCreateCommandQueue(m_context, m_device, NULL, &error);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't create OpenCL command queue, error code: ")) + QString::number(error));

			return false;
		}

		//

		QFile file(":/files/pixmap_waving.cl");

		if (!file.open(QIODevice::ReadOnly))
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't open file with sources of OpenCL program \"pixmapWaving.cl\"")));

			return false;
		}

		QTextStream stream(&file);

		QByteArray sources = stream.readAll().toAscii();

		file.close();

		const char *src_data = sources.data();
		size_t src_length = sources.size();

		m_program = clCreateProgramWithSource(m_context, 1, &src_data, &src_length, &error);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't create OpenCL program, error code: ")) + QString::number(error));

			return false;
		}

		char *options = "-cl-single-precision-constant -cl-fast-relaxed-math";

		error = clBuildProgram(m_program, 0, NULL, options, NULL, NULL);

		if (error != CL_SUCCESS)
		{
			char build_log[1024];

			clGetProgramBuildInfo(m_program, m_device, CL_PROGRAM_BUILD_LOG, sizeof(build_log), build_log, NULL);

			CLog::getInstance()->error(QString(QObject::tr("Can't build OpenCL program, build log: ")) + QString(build_log));

			return false;
		}

		//

		m_kernel = clCreateKernel(m_program, "pixmapWaving", &error);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't create OpenCL kernel \"pixmapWaving\", error code: ")) + QString::number(error));

			return false;
		}

		error = clGetKernelWorkGroupInfo(m_kernel, m_device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &m_kernel_work_group_size, 0);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't get OpenCL kernel work group size, error code: ")) + QString::number(error));

			return false;
		}

		//

		m_input_image = source;
		m_output_image = dest;

		m_image_width = width;
		m_image_height = height;

		m_global_work_size[0] = width;
		m_global_work_size[1] = height;

		m_local_work_size[0] = 256;
		m_local_work_size[1] = 1;

		m_buffer_size = sizeof(unsigned char) * width * height * 4;

		m_input_mem = clCreateBuffer(m_context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, m_buffer_size, m_input_image, &error);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't create OpenCL buffer for input, error code: ")) + QString::number(error));

			return false;
		}

		m_output_mem = clCreateBuffer(m_context, CL_MEM_WRITE_ONLY, sizeof(unsigned char) * width * height * 4, NULL, &error);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't create OpenCL buffer for output, error code: ")) + QString::number(error));

			return false;
		}

		CLog::getInstance()->information(QString(QObject::tr("Initialized OpenCL framework (target device: ")) + QString(device_type == CL_DEVICE_TYPE_GPU ? "GPU" : "CPU") + QString(")"));

		return true;
	}

	void COpenCLFramework::destroy()
	{
		cl_int error;

		if (m_kernel)
		{
			error = clReleaseKernel(m_kernel);

			if (error != CL_SUCCESS)
			{
				CLog::getInstance()->warning(QString(QObject::tr("Can't release OpenCL kernel, error code: ")) + QString::number(error));
			}

			m_kernel = nullptr;
		}

		if (m_program)
		{
			error = clReleaseProgram(m_program);

			if (error != CL_SUCCESS)
			{
				CLog::getInstance()->warning(QString(QObject::tr("Can't release OpenCL program, error code: ")) + QString::number(error));
			}

			m_program = nullptr;
		}

		if (m_input_mem)
		{
			error = clReleaseMemObject(m_input_mem);

			if (error != CL_SUCCESS)
			{
				CLog::getInstance()->warning(QString(QObject::tr("Can't release OpenCL input memory object, error code: ")) + QString::number(error));
			}

			m_input_mem = nullptr;
		}

		if (m_output_mem)
		{
			error = clReleaseMemObject(m_output_mem);

			if (error != CL_SUCCESS)
			{
				CLog::getInstance()->warning(QString(QObject::tr("Can't release OpenCL input memory object, error code: ")) + QString::number(error));
			}

			m_output_mem = nullptr;
		}

		if (m_queue)
		{
			error = clReleaseCommandQueue(m_queue);

			if (error != CL_SUCCESS)
			{
				CLog::getInstance()->warning(QString(QObject::tr("Can't release OpenCL queue, error code: ")) + QString::number(error));
			}

			m_queue = nullptr;
		}

		if (m_context)
		{
			error = clReleaseContext(m_context);

			if (error != CL_SUCCESS)
			{
				CLog::getInstance()->warning(QString(QObject::tr("Can't release OpenCL context, error code: ")) + QString::number(error));
			}

			m_context = nullptr;
		}

		CLog::getInstance()->information(QString(QObject::tr("Destroyed OpenCL framework")));
	}

	bool COpenCLFramework::execute(int* coords)
	{
		cl_int error;

		error = clSetKernelArg(m_kernel, 0, sizeof(cl_mem), &m_input_mem);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't create OpenCL kernel argument (0), error code: ")) + QString::number(error));

			return false;
		}

		error = clSetKernelArg(m_kernel, 1, sizeof(cl_mem), &m_output_mem);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't create OpenCL kernel argument (1), error code: ")) + QString::number(error));

			return false;
		}

		//

		error = clSetKernelArg(m_kernel, 2, sizeof(int), &m_image_width);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't create OpenCL kernel argument (1), error code: ")) + QString::number(error));

			return false;
		}

		error = clSetKernelArg(m_kernel, 3, sizeof(int), &m_image_height);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't create OpenCL kernel argument (1), error code: ")) + QString::number(error));

			return false;
		}

		//

		cl_mem coords_mem = clCreateBuffer(m_context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(int) * (m_image_width > m_image_height ? m_image_width : m_image_height), coords, &error);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't create OpenCL buffer (coords), error code: ")) + QString::number(error));

			return false;
		}

		error = clSetKernelArg(m_kernel, 4, sizeof(cl_mem), &coords_mem);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't create OpenCL kernel argument (2), error code: ")) + QString::number(error));

			return false;
		}

		//

		error = clEnqueueNDRangeKernel(m_queue, m_kernel, 2, NULL, m_global_work_size, NULL, 0, NULL, NULL);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't enqueue OpenCL kernel, error code: ")) + QString::number(error));

			return false;
		}

		error = clFinish(m_queue);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't finish (0) OpenCL queue, error code: ")) + QString::number(error));

			return false;
		}

		//

		error = clEnqueueReadBuffer(m_queue, m_output_mem, CL_TRUE, 0, m_buffer_size, m_output_image, 0, NULL, NULL);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't read output image from OpenCL queue, error code: ")) + QString::number(error));

			return false;
		}

		error = clFinish(m_queue);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->error(QString(QObject::tr("Can't finish (1) OpenCL queue, error code: ")) + QString::number(error));

			return false;
		}

		error = clReleaseMemObject(coords_mem);

		if (error != CL_SUCCESS)
		{
			CLog::getInstance()->warning(QString(QObject::tr("Can't release OpenCL coords memory object, error code: ")) + QString::number(error));
		}

		return true;
	}
}

#endif // VL_USE_OPENCL
