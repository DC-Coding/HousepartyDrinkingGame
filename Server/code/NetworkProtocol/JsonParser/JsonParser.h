/*! ###########################################################
 *
 * @file JsonParser.h
 *
 * @author Michael Hollmer
 *
 * @date 18.04.2020
 *
########################################################### */

#pragma once
#include <iostream>
#include <vector>
#include <fstream>

/*! ####################################
 * @class JsonObject
 *
 * @author Michael Hollmer
 *
 * @date 18.04.2020
 *
 * @brief Represents a json object
 *
#################################### */
class JsonObject
{
    public:

        /*!
        * @brief Creates a new json object
        */
        JsonObject(std::string data) { _objectData = data; }

        /*!
        * @brief Returns the data as a string
        */
        std::string GetObjectData() { return _objectData;  }

    protected:

        /*!
        * @brief String variable which stores the json object data
        */
        std::string _objectData;
};


/*! ####################################
 * @class JsonParser
 *
 * @author Michael Hollmer
 *
 * @date 18.04.2020
 *
 * @brief Parses to the json format and from this, to the string, so the NetworkProtocol can process it furthor
 *
#################################### */

class JsonParser
{
    public:

        static std::vector<JsonObject> DeserializeJson(std::string filePath)
        {
            std::string file;
            std::ifstream read(filePath.c_str());
            while (read.good())
            {
                file.push_back(read.get());
            }

            //Delete everything except of the "Assets" array in the json file
            file.erase(file.begin(), file.begin() + file.find('{') + 1);
            file.erase(file.begin(), file.begin() + file.find('{'));
            file.erase(file.begin() + file.find_last_of(']'), file.end());

            //Now we need to seperate the file into several json objects
            std::vector<JsonObject> objects;
        
            while (file.find('{') != std::string::npos)
            {
                std::string newObjectData = file;
            
                //Delete everything except of the current object block
                newObjectData.erase(newObjectData.begin(), newObjectData.begin() + newObjectData.find('{'));
                newObjectData.erase(newObjectData.begin() + newObjectData.find('}') + 1, newObjectData.end());

                //Push the new object 
                objects.push_back(JsonObject(newObjectData));

                //Now that we have processed the object block we can remove it from the file variable
                file.erase(file.begin(), file.begin() + file.find("}") + 1);
            }

            return objects;
        }
};