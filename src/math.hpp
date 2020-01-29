#pragma once

#include <cmath>
#include <mutex>

/** This function calculates a sine with an angle in degrees
 * 
 * @param deg The angle in Degrees
 * @return float The sine of that angle
 */
float sinDeg(float deg);

/** This function calculates a cosine with an angle in degrees
 * 
 * @param deg The angle in Degrees
 * @return float The cosine of that angle
 */
float cosDeg(float deg);

template <class T>
class securedVariable{
    private: 
        T value;
        std::mutex varlock;
    public:
        securedVariable(){
            value = 0;
        }
        securedVariable(T val){
            value = val;
        };

        T get(){
            std::lock_guard <std::mutex> lockguard(varlock);
            return value;
        };

        void operator=(T val){
            std::lock_guard <std::mutex> lockguard(varlock);
            this->value = val;
            return;
        }

        bool operator<(T val){
            std::lock_guard <std::mutex> lockguard(varlock);
            return this->value < val;
        }

        bool operator>(T val){
            std::lock_guard <std::mutex> lockguard(varlock);
            return this->value > val;
        }

        bool operator<=(T val){
            std::lock_guard <std::mutex> lockguard(varlock);
            return this->value <= val;
        }

        bool operator>=(T val){
            std::lock_guard <std::mutex> lockguard(varlock);
            return this->value >= val;
        }

        securedVariable operator+(securedVariable<T> val2){
            std::lock_guard <std::mutex> lockguard(varlock);
            securedVariable<T> result;
            result = this->get() + val2.get();
            return result;
        }

        securedVariable operator-(securedVariable<T> val2){
            std::lock_guard <std::mutex> lockguard(varlock);
            securedVariable<T> result;
            result = this->get() - val2.get();
            return result;
        }

        T abs(){
            std::lock_guard <std::mutex> lockguard(varlock);
            T result;
            if(value < 0){
                result = -value;
            }else{
                result = value;
            }
            return result;
        }



};
