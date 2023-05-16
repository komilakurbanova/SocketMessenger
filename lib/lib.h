#pragma once
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/access.hpp>

#include <string>
#include <set>
#include <vector>

struct User {
    std::string username;
    std::string name;
    std::string password_hash;
    std::string password_salt;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & username;
        ar & name;
        ar & password_hash;
        ar & password_salt;
    }
};

struct Message {
    std::string sender_username;
    std::string chat_id;
    std::string content;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & sender_username;
        ar & chat_id;
        ar & content;
    }
};

struct Chat {
    std::string chat_id;
    std::string chat_name;
    std::set<std::string> members;
    std::vector<Message> messages;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & chat_id;
        ar & chat_name;
        ar & members;
        ar & messages;
    }
};
