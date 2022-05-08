//
// Created by liwy on 2022/2/10.
//

#ifndef OBS_STUDIO_SINGLETON_FACTORY_H
#define OBS_STUDIO_SINGLETON_FACTORY_H
#include <mutex>
#include <memory>

template<typename T>
class SingletonFactory {
public:
	template<typename ...Args>
	static std::shared_ptr<T> GetInstance(Args&& ...args) {
		if (nullptr == instance_) {
			std::lock_guard<std::mutex> locker(mutex_);
			if (nullptr == instance_) {
				instance_ = std::make_shared<T>(std::forward<Args>(args)...);
			}
		}
		return instance_;
	}

	static void Release() {
		if (nullptr != instance_) {
			instance_.reset();
			instance_ = nullptr;
		}
	}
private:
	explicit SingletonFactory();
	SingletonFactory(const SingletonFactory&) = delete;
	SingletonFactory& operator=(const SingletonFactory&) = delete;
	~SingletonFactory();


private:
	static std::shared_ptr<T> instance_;
	static std::mutex mutex_;
};

template<typename T>
std::shared_ptr<T> SingletonFactory<T>::instance_ = nullptr;

template<typename T>
std::mutex SingletonFactory<T>::mutex_;


#endif //OBS_STUDIO_SINGLETON_FACTORY_H
