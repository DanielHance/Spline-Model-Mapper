#include "Resource.h"
#include <vector>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

struct Resource;

namespace red
{

	struct Resources
	{
	public:
		template <typename T>
		std::shared_ptr<T> load(std::string _path)
		{
			//Paths are diffrent on windows/web builds
			#ifdef __EMSCRIPTEN__
			_path = "data/" + _path;
			#else		
			_path = "../data/" + _path;
			#endif
			

			//Check if resource is in cache
			for (size_t i = 0; i < m_resources.size(); ++i)
			{
				//Return if found
				if (m_resources.at(i)->getPath() == _path)
				{
					std::shared_ptr<T> rtn = std::dynamic_pointer_cast<T>(m_resources.at(i));
					if (rtn) return rtn;
				}
			}

			//Create new instance, then cache it
			std::shared_ptr<T> rtn = std::make_shared<T>();
				rtn->m_path = _path;
				rtn->m_resources = std::make_shared<Resources>();
				rtn->load();
				m_resources.push_back(rtn);

				return rtn;

		}

		template <typename T>
		void reload(std::string _path)
		{

			for (size_t i = 0; i < m_resources.size(); i++)
			{
				if (m_resources.at(i)->getPath() == _path)
				{
					std::shared_ptr<T> resource = std::dynamic_pointer_cast<T>(m_resources.at(i));
					resource->m_path = "Invalid";
					return;
				}
			}
		}

	private:
		std::vector <std::shared_ptr<Resource>>m_resources; //cache of resources
	};

}