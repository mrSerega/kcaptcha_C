#include <boost/interprocess/windows_shared_memory.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <cstring>
#include <cstdlib>

#ifndef SHARED_MEMORY_H_28102016
#define SHARED_MEMORY_H_28102016

using namespace  boost::interprocess;

///класс для работы с shared_memory

class cSharedMemory{
	public:
		/**
		\todo
		*/
		cSharedMemory();
		/**
		\todo
		*/
		cSharedMemory(const char* shared_memory_object_name)
			:_shared_memory_object_name(shared_memory_object_name)
			{}
		~cSharedMemory();
		///создает объет shared memory в памяти
		void createObject(size_t size);
		///удаляет объект shared memory Из памяти
		/**
		\todo
		*/
		void removeObject();
		///задает данные в объект shared memory
		void setData(unsigned char* data, size_t size);
	private:
		const char* _shared_memory_object_name;
		windows_shared_memory* _windows_shared_memory_object;
		mapped_region* _mapped_region_object;
};

cSharedMemory::cSharedMemory(){
	//TODO
}

cSharedMemory::~cSharedMemory(){
	//delete &_windows_shared_memory_object;
	//delete &_mapped_region_object;
}

void cSharedMemory::createObject(size_t size){
	_windows_shared_memory_object = new windows_shared_memory(create_only, _shared_memory_object_name, read_write, size);
	_mapped_region_object =  new mapped_region(*(_windows_shared_memory_object), read_write);
}

void cSharedMemory::removeObject(){
	//TODO
}

void cSharedMemory::setData(unsigned char* data, size_t size){
    std::memcpy(_mapped_region_object->get_address(), data, size);
}

#endif
