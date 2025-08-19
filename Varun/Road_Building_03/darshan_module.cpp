#include"VK.h"

VkResult createVertexBuffer_by_darshan(void)
{
	// Local Variables
	VkResult vkResult = VK_SUCCESS;

	// code


	fprintf(DRSJ_gpLogFile, "%s %d : - - - - - For road border - - - - -\n", __func__, __LINE__);
	{
		// define triangle position array
		float rectangle_position[] = {
			// First Triangle
			-0.05f,1.0f,0.0f,	// Left-Top
			-1.0f,-1.0f,0.0f,	// Left-Bottom
			1.0f,-1.0f,0.0f,	// Right-Bottom
			// Second Triangle
			0.05f,1.0f,0.0f,		// Right-Top
			-0.05f,1.0f,0.0f,	// Left-Top
			1.0f,-1.0f,0.0f		// Right-Bottom
		};

		float rectangle_color[] = {

			0.2f,0.2f,0.2f,
			0.2f,0.2f,0.2f,
			0.2f,0.2f,0.2f,

			0.2f,0.2f,0.2f,
			0.2f,0.2f,0.2f,
			0.2f,0.2f,0.2f,

		};

		{
			memset((void*)&vertexData_position_road_border, 0, sizeof(VertexData));

			VkBufferCreateInfo vkBufferCreateInfo;
			memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

			vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vkBufferCreateInfo.pNext = NULL;
			vkBufferCreateInfo.flags = 0;
			vkBufferCreateInfo.size = sizeof(rectangle_position);
			vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			// In Vulkan memory alloation does not in bytes, but it is done in regions.
			// The max number of region is minumum 4096 
			// Vulkan demands/reccommend using small number of large size allocations and use them repetetively for diffeent resource

			vkResult = vkCreateBuffer(DRSJ_vkDevice, &vkBufferCreateInfo, NULL, &vertexData_position_road_border.vkBuffer);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() SUCCESSED\n", __func__, __LINE__);
			}

			VkMemoryRequirements vkMemoryRequirements;
			memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
			vkGetBufferMemoryRequirements(DRSJ_vkDevice, vertexData_position_road_border.vkBuffer, &vkMemoryRequirements);

			VkMemoryAllocateInfo vkMemoryAllocateInfo;
			memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

			vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			vkMemoryAllocateInfo.pNext = NULL;
			vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;	// Region wise size	
			vkMemoryAllocateInfo.memoryTypeIndex = 0;

			for (uint32_t iCounter = 0; iCounter < DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypeCount; iCounter++)
			{
				if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
				{
					if (DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypes[iCounter].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
					{
						vkMemoryAllocateInfo.memoryTypeIndex = iCounter;
						fprintf(DRSJ_gpLogFile, "%s %d : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT found at index : [%d] \n", __func__, __LINE__, iCounter);
						break;
					}
				}
				vkMemoryRequirements.memoryTypeBits >>= 1;
			}

			vkResult = vkAllocateMemory(DRSJ_vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_position_road_border.vkDeviceMemory);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() SUCCESSED\n", __func__, __LINE__);
			}

			vkResult = vkBindBufferMemory(DRSJ_vkDevice, vertexData_position_road_border.vkBuffer, vertexData_position_road_border.vkDeviceMemory, 0);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() SUCCESSED\n", __func__, __LINE__);
			}

			void* data = NULL;
			vkResult = vkMapMemory(DRSJ_vkDevice, vertexData_position_road_border.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() SUCCESSED\n", __func__, __LINE__);
			}

			memcpy(data, (void*)&rectangle_position, sizeof(rectangle_position));

			vkUnmapMemory(DRSJ_vkDevice, vertexData_position_road_border.vkDeviceMemory);
		}


		//
		// Vertex Color Buffer
		//

		{
			memset((void*)&vertexData_color_road_border, 0, sizeof(VertexData));

			VkBufferCreateInfo vkBufferCreateInfo;
			memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

			vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vkBufferCreateInfo.pNext = NULL;
			vkBufferCreateInfo.flags = 0;
			vkBufferCreateInfo.size = sizeof(rectangle_color);
			vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			// In Vulkan memory alloation does not in bytes, but it is done in regions.
			// The max number of region is minumum 4096 
			// Vulkan demands/reccommend using small number of large size allocations and use them repetetively for diffeent resource

			vkResult = vkCreateBuffer(DRSJ_vkDevice, &vkBufferCreateInfo, NULL, &vertexData_color_road_border.vkBuffer);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() SUCCESSED\n", __func__, __LINE__);
			}

			VkMemoryRequirements vkMemoryRequirements;
			memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
			vkGetBufferMemoryRequirements(DRSJ_vkDevice, vertexData_color_road_border.vkBuffer, &vkMemoryRequirements);

			VkMemoryAllocateInfo vkMemoryAllocateInfo;
			memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

			vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			vkMemoryAllocateInfo.pNext = NULL;
			vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;	// Region wise size	
			vkMemoryAllocateInfo.memoryTypeIndex = 0;

			for (uint32_t iCounter = 0; iCounter < DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypeCount; iCounter++)
			{
				if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
				{
					if (DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypes[iCounter].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
					{
						vkMemoryAllocateInfo.memoryTypeIndex = iCounter;
						fprintf(DRSJ_gpLogFile, "%s %d : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT found at index : [%d] \n", __func__, __LINE__, iCounter);
						break;
					}
				}
				vkMemoryRequirements.memoryTypeBits >>= 1;
			}

			vkResult = vkAllocateMemory(DRSJ_vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_color_road_border.vkDeviceMemory);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() SUCCESSED\n", __func__, __LINE__);
			}

			vkResult = vkBindBufferMemory(DRSJ_vkDevice, vertexData_color_road_border.vkBuffer, vertexData_color_road_border.vkDeviceMemory, 0);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() SUCCESSED\n", __func__, __LINE__);
			}

			void* data = NULL;
			vkResult = vkMapMemory(DRSJ_vkDevice, vertexData_color_road_border.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() SUCCESSED\n", __func__, __LINE__);
			}

			memcpy(data, (void*)&rectangle_color, sizeof(rectangle_color));

			vkUnmapMemory(DRSJ_vkDevice, vertexData_color_road_border.vkDeviceMemory);
		}
	}


	fprintf(DRSJ_gpLogFile, "%s %d : - - - - - For Main Road - - - - - \n", __func__, __LINE__);
	{
		// define triangle position array
		float rectangle_position[] = {
			// First Triangle
			-0.05f,1.0f,0.0f,	// Left-Top
			-1.0f,-1.0f,0.0f,	// Left-Bottom
			1.0f,-1.0f,0.0f,	// Right-Bottom
			// Second Triangle
			0.05f,1.0f,0.0f,		// Right-Top
			-0.05f,1.0f,0.0f,	// Left-Top
			1.0f,-1.0f,0.0f		// Right-Bottom
		};

		float rectangle_color[] = {

			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,

			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,

		};

		{
			memset((void*)&vertexData_position_main_road, 0, sizeof(VertexData));

			VkBufferCreateInfo vkBufferCreateInfo;
			memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

			vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vkBufferCreateInfo.pNext = NULL;
			vkBufferCreateInfo.flags = 0;
			vkBufferCreateInfo.size = sizeof(rectangle_position);
			vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			// In Vulkan memory alloation does not in bytes, but it is done in regions.
			// The max number of region is minumum 4096 
			// Vulkan demands/reccommend using small number of large size allocations and use them repetetively for diffeent resource

			vkResult = vkCreateBuffer(DRSJ_vkDevice, &vkBufferCreateInfo, NULL, &vertexData_position_main_road.vkBuffer);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() SUCCESSED\n", __func__, __LINE__);
			}

			VkMemoryRequirements vkMemoryRequirements;
			memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
			vkGetBufferMemoryRequirements(DRSJ_vkDevice, vertexData_position_main_road.vkBuffer, &vkMemoryRequirements);

			VkMemoryAllocateInfo vkMemoryAllocateInfo;
			memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

			vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			vkMemoryAllocateInfo.pNext = NULL;
			vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;	// Region wise size	
			vkMemoryAllocateInfo.memoryTypeIndex = 0;

			for (uint32_t iCounter = 0; iCounter < DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypeCount; iCounter++)
			{
				if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
				{
					if (DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypes[iCounter].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
					{
						vkMemoryAllocateInfo.memoryTypeIndex = iCounter;
						fprintf(DRSJ_gpLogFile, "%s %d : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT found at index : [%d] \n", __func__, __LINE__, iCounter);
						break;
					}
				}
				vkMemoryRequirements.memoryTypeBits >>= 1;
			}

			vkResult = vkAllocateMemory(DRSJ_vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_position_main_road.vkDeviceMemory);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() SUCCESSED\n", __func__, __LINE__);
			}

			//vkResult = vkBindBufferMemory(DRSJ_vkDevice, vertexData_position_main_road.vkBuffer, vertexData_position_main_road.vkDeviceMemory, vkMemoryAllocateInfo.allocationSize);
			vkResult = vkBindBufferMemory(DRSJ_vkDevice, vertexData_position_main_road.vkBuffer, vertexData_position_main_road.vkDeviceMemory, 0);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() SUCCESSED\n", __func__, __LINE__);
			}

			void* data = NULL;
			vkResult = vkMapMemory(DRSJ_vkDevice, vertexData_position_main_road.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() SUCCESSED\n", __func__, __LINE__);
			}

			memcpy(data, (void*)&rectangle_position, sizeof(rectangle_position));

			vkUnmapMemory(DRSJ_vkDevice, vertexData_position_main_road.vkDeviceMemory);
		}


		//
		// Vertex Color Buffer
		//

		{
			memset((void*)&vertexData_color_main_road, 0, sizeof(VertexData));

			VkBufferCreateInfo vkBufferCreateInfo;
			memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

			vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vkBufferCreateInfo.pNext = NULL;
			vkBufferCreateInfo.flags = 0;
			vkBufferCreateInfo.size = sizeof(rectangle_color);
			vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			// In Vulkan memory alloation does not in bytes, but it is done in regions.
			// The max number of region is minumum 4096 
			// Vulkan demands/reccommend using small number of large size allocations and use them repetetively for diffeent resource

			vkResult = vkCreateBuffer(DRSJ_vkDevice, &vkBufferCreateInfo, NULL, &vertexData_color_main_road.vkBuffer);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() SUCCESSED\n", __func__, __LINE__);
			}

			VkMemoryRequirements vkMemoryRequirements;
			memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
			vkGetBufferMemoryRequirements(DRSJ_vkDevice, vertexData_color_main_road.vkBuffer, &vkMemoryRequirements);

			VkMemoryAllocateInfo vkMemoryAllocateInfo;
			memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

			vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			vkMemoryAllocateInfo.pNext = NULL;
			vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;	// Region wise size	
			vkMemoryAllocateInfo.memoryTypeIndex = 0;

			for (uint32_t iCounter = 0; iCounter < DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypeCount; iCounter++)
			{
				if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
				{
					if (DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypes[iCounter].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
					{
						vkMemoryAllocateInfo.memoryTypeIndex = iCounter;
						fprintf(DRSJ_gpLogFile, "%s %d : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT found at index : [%d] \n", __func__, __LINE__, iCounter);
						break;
					}
				}
				vkMemoryRequirements.memoryTypeBits >>= 1;
			}

			vkResult = vkAllocateMemory(DRSJ_vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_color_main_road.vkDeviceMemory);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() SUCCESSED\n", __func__, __LINE__);
			}

			vkResult = vkBindBufferMemory(DRSJ_vkDevice, vertexData_color_main_road.vkBuffer, vertexData_color_main_road.vkDeviceMemory, 0);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() SUCCESSED\n", __func__, __LINE__);
			}

			void* data = NULL;
			vkResult = vkMapMemory(DRSJ_vkDevice, vertexData_color_main_road.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() SUCCESSED\n", __func__, __LINE__);
			}

			memcpy(data, (void*)&rectangle_color, sizeof(rectangle_color));

			vkUnmapMemory(DRSJ_vkDevice, vertexData_color_main_road.vkDeviceMemory);
		}
	}


	fprintf(DRSJ_gpLogFile, "%s %d : - - - - - For zebra_crossing- - - - - \n", __func__, __LINE__);
	{
		// define triangle position array
		float rectangle_position[] = {
			// First Triangle
			-0.25f,1.0f,0.0f,	// Left-Top
			-1.0f,-1.0f,0.0f,	// Left-Bottom
			1.0f,-1.0f,0.0f,	// Right-Bottom
			// Second Triangle
			0.25f,1.0f,0.0f,		// Right-Top
			-0.25f,1.0f,0.0f,	// Left-Top
			1.0f,-1.0f,0.0f		// Right-Bottom
		};

		float rectangle_color[] = {

		1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,

			1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,

		};

		{
			memset((void*)&vertexData_position_zebra_crossing, 0, sizeof(VertexData));

			VkBufferCreateInfo vkBufferCreateInfo;
			memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

			vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vkBufferCreateInfo.pNext = NULL;
			vkBufferCreateInfo.flags = 0;
			vkBufferCreateInfo.size = sizeof(rectangle_position);
			vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			// In Vulkan memory alloation does not in bytes, but it is done in regions.
			// The max number of region is minumum 4096 
			// Vulkan demands/reccommend using small number of large size allocations and use them repetetively for diffeent resource

			vkResult = vkCreateBuffer(DRSJ_vkDevice, &vkBufferCreateInfo, NULL, &vertexData_position_zebra_crossing.vkBuffer);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() SUCCESSED\n", __func__, __LINE__);
			}

			VkMemoryRequirements vkMemoryRequirements;
			memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
			vkGetBufferMemoryRequirements(DRSJ_vkDevice, vertexData_position_zebra_crossing.vkBuffer, &vkMemoryRequirements);

			VkMemoryAllocateInfo vkMemoryAllocateInfo;
			memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

			vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			vkMemoryAllocateInfo.pNext = NULL;
			vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;	// Region wise size	
			vkMemoryAllocateInfo.memoryTypeIndex = 0;

			for (uint32_t iCounter = 0; iCounter < DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypeCount; iCounter++)
			{
				if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
				{
					if (DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypes[iCounter].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
					{
						vkMemoryAllocateInfo.memoryTypeIndex = iCounter;
						fprintf(DRSJ_gpLogFile, "%s %d : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT found at index : [%d] \n", __func__, __LINE__, iCounter);
						break;
					}
				}
				vkMemoryRequirements.memoryTypeBits >>= 1;
			}

			vkResult = vkAllocateMemory(DRSJ_vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_position_zebra_crossing.vkDeviceMemory);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() SUCCESSED\n", __func__, __LINE__);
			}

			//vkResult = vkBindBufferMemory(DRSJ_vkDevice, vertexData_position_main_road.vkBuffer, vertexData_position_main_road.vkDeviceMemory, vkMemoryAllocateInfo.allocationSize);
			vkResult = vkBindBufferMemory(DRSJ_vkDevice, vertexData_position_zebra_crossing.vkBuffer, vertexData_position_zebra_crossing.vkDeviceMemory, 0);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() SUCCESSED\n", __func__, __LINE__);
			}

			void* data = NULL;
			vkResult = vkMapMemory(DRSJ_vkDevice, vertexData_position_zebra_crossing.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() SUCCESSED\n", __func__, __LINE__);
			}

			memcpy(data, (void*)&rectangle_position, sizeof(rectangle_position));

			vkUnmapMemory(DRSJ_vkDevice, vertexData_position_zebra_crossing.vkDeviceMemory);
		}


		//
		// Vertex Color Buffer
		//

		{
			memset((void*)&vertexData_color_zebra_crossing, 0, sizeof(VertexData));

			VkBufferCreateInfo vkBufferCreateInfo;
			memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

			vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vkBufferCreateInfo.pNext = NULL;
			vkBufferCreateInfo.flags = 0;
			vkBufferCreateInfo.size = sizeof(rectangle_color);
			vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			// In Vulkan memory alloation does not in bytes, but it is done in regions.
			// The max number of region is minumum 4096 
			// Vulkan demands/reccommend using small number of large size allocations and use them repetetively for diffeent resource

			vkResult = vkCreateBuffer(DRSJ_vkDevice, &vkBufferCreateInfo, NULL, &vertexData_color_zebra_crossing.vkBuffer);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() SUCCESSED\n", __func__, __LINE__);
			}

			VkMemoryRequirements vkMemoryRequirements;
			memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
			vkGetBufferMemoryRequirements(DRSJ_vkDevice, vertexData_color_zebra_crossing.vkBuffer, &vkMemoryRequirements);

			VkMemoryAllocateInfo vkMemoryAllocateInfo;
			memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

			vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			vkMemoryAllocateInfo.pNext = NULL;
			vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;	// Region wise size	
			vkMemoryAllocateInfo.memoryTypeIndex = 0;

			for (uint32_t iCounter = 0; iCounter < DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypeCount; iCounter++)
			{
				if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
				{
					if (DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypes[iCounter].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
					{
						vkMemoryAllocateInfo.memoryTypeIndex = iCounter;
						fprintf(DRSJ_gpLogFile, "%s %d : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT found at index : [%d] \n", __func__, __LINE__, iCounter);
						break;
					}
				}
				vkMemoryRequirements.memoryTypeBits >>= 1;
			}

			vkResult = vkAllocateMemory(DRSJ_vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_color_zebra_crossing.vkDeviceMemory);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() SUCCESSED\n", __func__, __LINE__);
			}

			vkResult = vkBindBufferMemory(DRSJ_vkDevice, vertexData_color_zebra_crossing.vkBuffer, vertexData_color_zebra_crossing.vkDeviceMemory, 0);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() SUCCESSED\n", __func__, __LINE__);
			}

			void* data = NULL;
			vkResult = vkMapMemory(DRSJ_vkDevice, vertexData_color_zebra_crossing.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() SUCCESSED\n", __func__, __LINE__);
			}

			memcpy(data, (void*)&rectangle_color, sizeof(rectangle_color));

			vkUnmapMemory(DRSJ_vkDevice, vertexData_color_zebra_crossing.vkDeviceMemory);
		}
	}



	return vkResult;
}


VkResult createUniformBuffer_by_darshan(void)
{
	VkResult vkResult = VK_SUCCESS;

	memset((void*)&uniformData_road_border, 0, sizeof(UniformData));
	memset((void*)&uniformData_main_road, 0, sizeof(UniformData));
	memset((void*)&uniformData_zebra_crossing, 0, sizeof(UniformData));

	VkBufferCreateInfo vkBufferCreateInfo;
	memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

	vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vkBufferCreateInfo.pNext = NULL;
	vkBufferCreateInfo.flags = 0;
	vkBufferCreateInfo.size = sizeof(MyUniformData);
	vkBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	vkBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vkBufferCreateInfo.queueFamilyIndexCount = 0;
	vkBufferCreateInfo.pQueueFamilyIndices = NULL;
	// For road border
	fprintf(DRSJ_gpLogFile, "%s %d : - - - - - For road border - - - - -\n", __func__, __LINE__);
	{
		vkResult = vkCreateBuffer(DRSJ_vkDevice, &vkBufferCreateInfo, NULL, &uniformData_road_border.vkBuffer);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() SUCCESSED\n", __func__, __LINE__);
		}

		VkMemoryRequirements vkMemoryRequirements;
		memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
		vkGetBufferMemoryRequirements(DRSJ_vkDevice, uniformData_road_border.vkBuffer, &vkMemoryRequirements);

		VkMemoryAllocateInfo vkMemoryAllocateInfo;
		memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

		vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		vkMemoryAllocateInfo.pNext = NULL;
		vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;	// Region wise size	
		vkMemoryAllocateInfo.memoryTypeIndex = 0;

		for (uint32_t iCounter = 0; iCounter < DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypeCount; iCounter++)
		{
			if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
			{
				if (DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypes[iCounter].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
				{
					vkMemoryAllocateInfo.memoryTypeIndex = iCounter;
					fprintf(DRSJ_gpLogFile, "%s %d : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT found at index : [%d] \n", __func__, __LINE__, iCounter);
					break;
				}
			}
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}

		vkResult = vkAllocateMemory(DRSJ_vkDevice, &vkMemoryAllocateInfo, NULL, &uniformData_road_border.vkDeviceMemory);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() SUCCESSED\n", __func__, __LINE__);
		}

		vkResult = vkBindBufferMemory(DRSJ_vkDevice, uniformData_road_border.vkBuffer, uniformData_road_border.vkDeviceMemory, 0);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() SUCCESSED\n", __func__, __LINE__);
		}
	}

	// For Main Road
	fprintf(DRSJ_gpLogFile, "%s %d : - - - - - For Main Road - - - - - \n", __func__, __LINE__);
	if(0)
	{
		vkResult = vkCreateBuffer(DRSJ_vkDevice, &vkBufferCreateInfo, NULL, &uniformData_main_road.vkBuffer);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() SUCCESSED\n", __func__, __LINE__);
		}

		VkMemoryRequirements vkMemoryRequirements;
		memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
		vkGetBufferMemoryRequirements(DRSJ_vkDevice, uniformData_main_road.vkBuffer, &vkMemoryRequirements);

		VkMemoryAllocateInfo vkMemoryAllocateInfo;
		memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

		vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		vkMemoryAllocateInfo.pNext = NULL;
		vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;	// Region wise size	
		vkMemoryAllocateInfo.memoryTypeIndex = 0;

		for (uint32_t iCounter = 0; iCounter < DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypeCount; iCounter++)
		{
			if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
			{
				if (DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypes[iCounter].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
				{
					vkMemoryAllocateInfo.memoryTypeIndex = iCounter;
					fprintf(DRSJ_gpLogFile, "%s %d : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT found at index : [%d] \n", __func__, __LINE__, iCounter);
					break;
				}
			}
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}

		vkResult = vkAllocateMemory(DRSJ_vkDevice, &vkMemoryAllocateInfo, NULL, &uniformData_main_road.vkDeviceMemory);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() SUCCESSED\n", __func__, __LINE__);
		}

		vkResult = vkBindBufferMemory(DRSJ_vkDevice, uniformData_main_road.vkBuffer, uniformData_main_road.vkDeviceMemory, 0);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() SUCCESSED\n", __func__, __LINE__);
		}
	}


	fprintf(DRSJ_gpLogFile, "%s %d : - - - - - For zebra_crossing- - - - - \n", __func__, __LINE__);
	if(0)
	{
		
		vkResult = vkCreateBuffer(DRSJ_vkDevice, &vkBufferCreateInfo, NULL, &uniformData_zebra_crossing.vkBuffer);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() SUCCESSED\n", __func__, __LINE__);
		}

		VkMemoryRequirements vkMemoryRequirements;
		memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
		vkGetBufferMemoryRequirements(DRSJ_vkDevice, uniformData_zebra_crossing.vkBuffer, &vkMemoryRequirements);

		VkMemoryAllocateInfo vkMemoryAllocateInfo;
		memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

		vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		vkMemoryAllocateInfo.pNext = NULL;
		vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;	// Region wise size	
		vkMemoryAllocateInfo.memoryTypeIndex = 0;

		for (uint32_t iCounter = 0; iCounter < DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypeCount; iCounter++)
		{
			if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
			{
				if (DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypes[iCounter].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
				{
					vkMemoryAllocateInfo.memoryTypeIndex = iCounter;
					fprintf(DRSJ_gpLogFile, "%s %d : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT found at index : [%d] \n", __func__, __LINE__, iCounter);
					break;
				}
			}
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}

		vkResult = vkAllocateMemory(DRSJ_vkDevice, &vkMemoryAllocateInfo, NULL, &uniformData_zebra_crossing.vkDeviceMemory);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() SUCCESSED\n", __func__, __LINE__);
		}

		vkResult = vkBindBufferMemory(DRSJ_vkDevice, uniformData_zebra_crossing.vkBuffer, uniformData_zebra_crossing.vkDeviceMemory, 0);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() SUCCESSED\n", __func__, __LINE__);
		}
	}

	return vkResult;
}


VkResult updateUniformBuffer_by_darshan(void)
{
	// code
	VkResult vkResult = VK_SUCCESS;
	MyUniformData myUniformData;
	void* data = NULL;
	glm::mat4 perspectiveProjectionMatrix;

	// For road border
	pushConstant_model_uniformData[0].modelMatrix = glm::mat4(1.0f);
	pushConstant_model_uniformData[0].modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -1.25, -6.0)) * glm::scale(glm::mat4(1.0), glm::vec3(2.75, 1.25, 1.0));

	// For Main Road
	pushConstant_model_uniformData[1].modelMatrix = glm::mat4(1.0f);
	pushConstant_model_uniformData[1].modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -1.25, -6.0)) * glm::scale(glm::mat4(1.0), glm::vec3(2.25, 1.25, 1.0));

	// For zebra_crossing
	pushConstant_model_uniformData[2].modelMatrix = glm::mat4(1.0f);
	pushConstant_model_uniformData[2].modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, -1.25, -6.0)) * glm::scale(glm::mat4(1.0), glm::vec3(0.03, 1.25, 1.0));


	{
		memset((void*)&myUniformData, 0, sizeof(MyUniformData));

		myUniformData.viewMatrix = glm::mat4(1.0);
		perspectiveProjectionMatrix = glm::mat4(1.0);

		perspectiveProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
		perspectiveProjectionMatrix[1][1] = perspectiveProjectionMatrix[1][1] * (-1.0f);
		myUniformData.projectionMatrix = perspectiveProjectionMatrix;

		// Map uniform buffer
		vkResult = vkMapMemory(DRSJ_vkDevice, uniformData_road_border.vkDeviceMemory, 0, sizeof(MyUniformData), 0, &data);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() Triangle FAILED \n", __func__, __LINE__);
			return (vkResult);
		}

		memcpy(data, (void*)&myUniformData, sizeof(MyUniformData));

		vkUnmapMemory(DRSJ_vkDevice, uniformData_road_border.vkDeviceMemory);
	}

	return vkResult;
}

void buildCommandBuffers_by_darshan(uint32_t index)
{
	// For road border
	{


		vkCmdPushConstants(DRSJ_vkCommandBuffer_array[index],
			vkPipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT,
			0,
			sizeof(glm::mat4),
			&pushConstant_model_uniformData[0]
		);

		// Bind with the position vertex buffer
		VkDeviceSize vkDeviceSize_offset_position[1];
		memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));

		vkCmdBindVertexBuffers(DRSJ_vkCommandBuffer_array[index], 0, 1, &(vertexData_position_road_border.vkBuffer), vkDeviceSize_offset_position);

		// Bind with the color vertex buffer
		VkDeviceSize vkDeviceSize_offset_color[1];
		memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));

		vkCmdBindVertexBuffers(DRSJ_vkCommandBuffer_array[index], 1, 1, &(vertexData_color_road_border.vkBuffer), vkDeviceSize_offset_color);

		//******************************************************//
		//														//
		// --- Here we should call Vulkan drawing function	---	//
		//														//
		//******************************************************//

		vkCmdDraw(DRSJ_vkCommandBuffer_array[index], 6, 1, 0, 1);

	}

	// For Main Road
	{

		vkCmdPushConstants(DRSJ_vkCommandBuffer_array[index],
			vkPipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT,
			0,
			sizeof(glm::mat4),
			&pushConstant_model_uniformData[1]
		);

		// Bind with the position vertex buffer
		VkDeviceSize vkDeviceSize_offset_position[1];
		memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));

		vkCmdBindVertexBuffers(DRSJ_vkCommandBuffer_array[index], 0, 1, &(vertexData_position_main_road.vkBuffer), vkDeviceSize_offset_position);

		// Bind with the color vertex buffer
		VkDeviceSize vkDeviceSize_offset_color[1];
		memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));

		vkCmdBindVertexBuffers(DRSJ_vkCommandBuffer_array[index], 1, 1, &(vertexData_color_main_road.vkBuffer), vkDeviceSize_offset_color);

		//******************************************************//
		//														//
		// --- Here we should call Vulkan drawing function	---	//
		//														//
		//******************************************************//

		vkCmdDraw(DRSJ_vkCommandBuffer_array[index], 6, 1, 0, 1);
	}

	// For zebra_crossing
	{
		vkCmdPushConstants(DRSJ_vkCommandBuffer_array[index],
			vkPipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT,
			0,
			sizeof(glm::mat4),
			&pushConstant_model_uniformData[2]
		);

		// Bind with the position vertex buffer
		VkDeviceSize vkDeviceSize_offset_position[1];
		memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));

		vkCmdBindVertexBuffers(DRSJ_vkCommandBuffer_array[index], 0, 1, &(vertexData_position_zebra_crossing.vkBuffer), vkDeviceSize_offset_position);

		// Bind with the color vertex buffer
		VkDeviceSize vkDeviceSize_offset_color[1];
		memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));

		vkCmdBindVertexBuffers(DRSJ_vkCommandBuffer_array[index], 1, 1, &(vertexData_color_zebra_crossing.vkBuffer), vkDeviceSize_offset_color);

		//******************************************************//
		//														//
		// --- Here we should call Vulkan drawing function	---	//
		//														//
		//******************************************************//


		vkCmdDraw(DRSJ_vkCommandBuffer_array[index], 6, 1, 0, 1);
	}

}


VkResult createVertexBuffer_2(void)
{
	// local:
	VkResult vkResult = VK_SUCCESS;


	// code:

	//triangle position array:
	float triangle_position[] =
	{
		0.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,
		1.0f,-1.0f,0.0f,

		-1.0f,0.5f,0.0f,
		0.0f,-1.5f,0.0f,
		1.0f,0.5f,0.0f	
	};

	//triangle color array:
	float triangle_color[] =
	{
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,

		0.0f,1.0f,1.0f,
		1.0f,0.0f,1.0f,
		1.0f,1.0f,0.0f
	};

	{
			memset((void*)&vertexData_position_2, 0, sizeof(VertexData));

			VkBufferCreateInfo vkBufferCreateInfo;
			memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

			vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vkBufferCreateInfo.pNext = NULL;
			vkBufferCreateInfo.flags = 0;
			vkBufferCreateInfo.size = sizeof(triangle_position);
			vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			// In Vulkan memory alloation does not in bytes, but it is done in regions.
			// The max number of region is minumum 4096 
			// Vulkan demands/reccommend using small number of large size allocations and use them repetetively for diffeent resource

			vkResult = vkCreateBuffer(DRSJ_vkDevice, &vkBufferCreateInfo, NULL, &vertexData_position_2.vkBuffer);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() SUCCESSED\n", __func__, __LINE__);
			}

			VkMemoryRequirements vkMemoryRequirements;
			memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
			vkGetBufferMemoryRequirements(DRSJ_vkDevice, vertexData_position_2.vkBuffer, &vkMemoryRequirements);

			VkMemoryAllocateInfo vkMemoryAllocateInfo;
			memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

			vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			vkMemoryAllocateInfo.pNext = NULL;
			vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;	// Region wise size	
			vkMemoryAllocateInfo.memoryTypeIndex = 0;

			for (uint32_t iCounter = 0; iCounter < DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypeCount; iCounter++)
			{
				if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
				{
					if (DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypes[iCounter].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
					{
						vkMemoryAllocateInfo.memoryTypeIndex = iCounter;
						fprintf(DRSJ_gpLogFile, "%s %d : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT found at index : [%d] \n", __func__, __LINE__, iCounter);
						break;
					}
				}
				vkMemoryRequirements.memoryTypeBits >>= 1;
			}

			vkResult = vkAllocateMemory(DRSJ_vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_position_2.vkDeviceMemory);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() SUCCESSED\n", __func__, __LINE__);
			}

			vkResult = vkBindBufferMemory(DRSJ_vkDevice, vertexData_position_2.vkBuffer, vertexData_position_2.vkDeviceMemory, 0);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() SUCCESSED\n", __func__, __LINE__);
			}

			void* data = NULL;
			vkResult = vkMapMemory(DRSJ_vkDevice, vertexData_position_2.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
			if (VK_SUCCESS != vkResult)
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() FAILED \n", __func__, __LINE__);
				return (vkResult);
			}
			else
			{
				fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() SUCCESSED\n", __func__, __LINE__);
			}

			memcpy(data, (void*)&triangle_position, sizeof(triangle_position));

			vkUnmapMemory(DRSJ_vkDevice, vertexData_position_2.vkDeviceMemory);
		}


	//
	// Vertex Color Buffer
	//

	{
		memset((void*)&vertexData_color_2 ,0, sizeof(VertexData));

		VkBufferCreateInfo vkBufferCreateInfo;
		memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

		vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vkBufferCreateInfo.pNext = NULL;
		vkBufferCreateInfo.flags = 0;
		vkBufferCreateInfo.size = sizeof(triangle_color);
		vkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		// In Vulkan memory alloation does not in bytes, but it is done in regions.
		// The max number of region is minumum 4096 
		// Vulkan demands/reccommend using small number of large size allocations and use them repetetively for diffeent resource

		vkResult = vkCreateBuffer(DRSJ_vkDevice, &vkBufferCreateInfo, NULL, &vertexData_color_2.vkBuffer);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() SUCCESSED\n", __func__, __LINE__);
		}

		VkMemoryRequirements vkMemoryRequirements;
		memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
		vkGetBufferMemoryRequirements(DRSJ_vkDevice, vertexData_color_2.vkBuffer, &vkMemoryRequirements);

		VkMemoryAllocateInfo vkMemoryAllocateInfo;
		memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

		vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		vkMemoryAllocateInfo.pNext = NULL;
		vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;	// Region wise size	
		vkMemoryAllocateInfo.memoryTypeIndex = 0;

		for (uint32_t iCounter = 0; iCounter < DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypeCount; iCounter++)
		{
			if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
			{
				if (DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypes[iCounter].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
				{
					vkMemoryAllocateInfo.memoryTypeIndex = iCounter;
					fprintf(DRSJ_gpLogFile, "%s %d : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT found at index : [%d] \n", __func__, __LINE__, iCounter);
					break;
				}
			}
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}

		vkResult = vkAllocateMemory(DRSJ_vkDevice, &vkMemoryAllocateInfo, NULL, &vertexData_color_2.vkDeviceMemory);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() SUCCESSED\n", __func__, __LINE__);
		}

		vkResult = vkBindBufferMemory(DRSJ_vkDevice, vertexData_color_2.vkBuffer, vertexData_color_2.vkDeviceMemory, 0);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() SUCCESSED\n", __func__, __LINE__);
		}

		void* data = NULL;
		vkResult = vkMapMemory(DRSJ_vkDevice, vertexData_color_2.vkDeviceMemory, 0, vkMemoryAllocateInfo.allocationSize, 0, &data);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() SUCCESSED\n", __func__, __LINE__);
		}

		memcpy(data, (void*)&triangle_color, sizeof(triangle_color));

		vkUnmapMemory(DRSJ_vkDevice, vertexData_color_2.vkDeviceMemory);
	}
	


	return (vkResult);
}

VkResult createUniformBuffer_2(void)
{
	VkResult vkResult = VK_SUCCESS;

	memset((void*)&uniformData_2, 0, sizeof(UniformData));

	VkBufferCreateInfo vkBufferCreateInfo;
	memset((void*)&vkBufferCreateInfo, 0, sizeof(VkBufferCreateInfo));

	vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	vkBufferCreateInfo.pNext = NULL;
	vkBufferCreateInfo.flags = 0;
	vkBufferCreateInfo.size = sizeof(MyUniformData);
	vkBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	vkBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	vkBufferCreateInfo.queueFamilyIndexCount = 0;
	vkBufferCreateInfo.pQueueFamilyIndices = NULL;

	// For START
	fprintf(DRSJ_gpLogFile, "%s %d : - - - - - For START - - - - -\n", __func__, __LINE__);

	if(0)
	{
		vkResult = vkCreateBuffer(DRSJ_vkDevice, &vkBufferCreateInfo, NULL, &uniformData_2.vkBuffer);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkCreateBuffer() SUCCESSED\n", __func__, __LINE__);
		}

		VkMemoryRequirements vkMemoryRequirements;
		memset((void*)&vkMemoryRequirements, 0, sizeof(VkMemoryRequirements));
		vkGetBufferMemoryRequirements(DRSJ_vkDevice, uniformData_2.vkBuffer, &vkMemoryRequirements);

		VkMemoryAllocateInfo vkMemoryAllocateInfo;
		memset((void*)&vkMemoryAllocateInfo, 0, sizeof(VkMemoryAllocateInfo));

		vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		vkMemoryAllocateInfo.pNext = NULL;
		vkMemoryAllocateInfo.allocationSize = vkMemoryRequirements.size;	// Region wise size	
		vkMemoryAllocateInfo.memoryTypeIndex = 0;

		for (uint32_t iCounter = 0; iCounter < DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypeCount; iCounter++)
		{
			if ((vkMemoryRequirements.memoryTypeBits & 1) == 1)
			{
				if (DRSJ_vkPhysicalDeviceMemoryProperties.memoryTypes[iCounter].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
				{
					vkMemoryAllocateInfo.memoryTypeIndex = iCounter;
					fprintf(DRSJ_gpLogFile, "%s %d : VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT found at index : [%d] \n", __func__, __LINE__, iCounter);
					break;
				}
			}
			vkMemoryRequirements.memoryTypeBits >>= 1;
		}

		vkResult = vkAllocateMemory(DRSJ_vkDevice, &vkMemoryAllocateInfo, NULL, &uniformData_2.vkDeviceMemory);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkAllocateMemory() SUCCESSED\n", __func__, __LINE__);
		}

		vkResult = vkBindBufferMemory(DRSJ_vkDevice, uniformData_2.vkBuffer, uniformData_2.vkDeviceMemory, 0);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() FAILED \n", __func__, __LINE__);
			return (vkResult);
		}
		else
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkBindBufferMemory() SUCCESSED\n", __func__, __LINE__);
		}
	}

	

	return vkResult;
}

VkResult buildCommandBuffers_2(uint32_t index_)
{
	// local:

	VkResult vkResult_vvg = VK_SUCCESS;

	fopen_s(&gpFile, "Log.txt", "a");
	fprintf(gpFile, " ##   Entered  %s() at LINE %d \n ", __func__, __LINE__);
	fclose(gpFile);

	// code:
	{


		vkCmdPushConstants(DRSJ_vkCommandBuffer_array[index_],
			vkPipelineLayout,
			VK_SHADER_STAGE_VERTEX_BIT,
			0,
			sizeof(glm::mat4),
			&pushConstant_model_uniformData[3]
		);

		// Bind with the position vertex buffer
		VkDeviceSize vkDeviceSize_offset_position[1];
		memset((void*)vkDeviceSize_offset_position, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_position));

		vkCmdBindVertexBuffers(DRSJ_vkCommandBuffer_array[index_], 0, 1, &(vertexData_position_2.vkBuffer), vkDeviceSize_offset_position);

		// Bind with the color vertex buffer
		VkDeviceSize vkDeviceSize_offset_color[1];
		memset((void*)vkDeviceSize_offset_color, 0, sizeof(VkDeviceSize) * _ARRAYSIZE(vkDeviceSize_offset_color));

		vkCmdBindVertexBuffers(DRSJ_vkCommandBuffer_array[index_], 1, 1, &(vertexData_color_2.vkBuffer), vkDeviceSize_offset_color);

		//******************************************************//
		//														//
		// --- Here we should call Vulkan drawing function	---	//
		//														//
		//******************************************************//

		vkCmdDraw(DRSJ_vkCommandBuffer_array[index_], 6, 1, 0, 1);

	}

	return (vkResult_vvg);
}

VkResult updateUniformBuffer_2(void)
{
	// code
	VkResult vkResult = VK_SUCCESS;
	MyUniformData myUniformData;
	void* data = NULL;
	glm::mat4 perspectiveProjectionMatrix;

	// For START SHAPE :

	pushConstant_model_uniformData[3].modelMatrix = glm::mat4(1.0f);
	pushConstant_model_uniformData[3].modelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -6.0f)) * glm::scale(glm::mat4(1.0), glm::vec3(1.0f,1.0f, 1.0f));

	
	{
		memset((void*)&myUniformData, 0, sizeof(MyUniformData));

		myUniformData.viewMatrix = glm::mat4(1.0);
		perspectiveProjectionMatrix = glm::mat4(1.0);

		perspectiveProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
		perspectiveProjectionMatrix[1][1] = perspectiveProjectionMatrix[1][1] * (-1.0f);
		myUniformData.projectionMatrix = perspectiveProjectionMatrix;

		// Map uniform buffer
		vkResult = vkMapMemory(DRSJ_vkDevice, uniformData_road_border.vkDeviceMemory, 0, sizeof(MyUniformData), 0, &data);
		if (VK_SUCCESS != vkResult)
		{
			fprintf(DRSJ_gpLogFile, "%s %d : vkMapMemory() Triangle FAILED \n", __func__, __LINE__);
			return (vkResult);
		}

		memcpy(data, (void*)&myUniformData, sizeof(MyUniformData));

		vkUnmapMemory(DRSJ_vkDevice, uniformData_road_border.vkDeviceMemory);
	}


	return (vkResult);
}



