#include "Node.h"
#include <cstring>
#include <utility>

Node::Node()
{
    type_ = NodeType::Invalid;
    value_.str = nullptr;
}

Node::~Node()
{
    this->CheckDeleteString();
}

Node::Node(const Node& node) : Node()
{
    switch (node.type_) {
        case NodeType::Char:
            this->SetChar(node.GetChar());
            break;
        case NodeType::Short:
            this->SetShort(node.GetShort());
            break;
        case NodeType::Int:
            this->SetInt(node.GetInt());
            break;
        case NodeType::LongLong:   
            this->SetLongLong(node.GetLongLong());
            break;
        case NodeType::String:
            this->SetString(node.GetString());
            break;
        default:
            break;
    }
}

Node& Node::operator=(const Node &node)
{
    if (&node == this)
        return *this;

    switch (node.type_) {
        case NodeType::Char:
            this->SetChar(node.GetChar());
            break;
        case NodeType::Short:
            this->SetShort(node.GetShort());
            break;
        case NodeType::Int:
            this->SetInt(node.GetInt());
            break;
        case NodeType::LongLong:   
            this->SetLongLong(node.GetLongLong());
            break;
        case NodeType::String:
            this->SetString(node.GetString());
            break;
        default:
            break;
    }

    return *this;
}

void Node::SetValue(const char *v)
{
    switch (type_) {
        case NodeType::Char:
            this->SetChar(std::stoi(v));
            break;
        case NodeType::Short:
            this->SetShort(std::stoi(v));
            break;
        case NodeType::Int:
            this->SetInt(std::stoi(v));
            break;
        case NodeType::LongLong:   
            this->SetLongLong(std::stoll(v));
            break;
        case NodeType::String:
            this->SetString(v);
            break;
        default:
            break;
    }
}

std::string Node::GetValue() const
{
    switch (type_) {
        case NodeType::Char:
            return std::to_string(this->GetChar());
        case NodeType::Short:
            return std::to_string(this->GetShort());
        case NodeType::Int:
            return std::to_string(this->GetInt());
        case NodeType::LongLong:   
            return std::to_string(this->GetLongLong());
        case NodeType::String:
            return this->GetString();
        default:
            return "";
    }
}

void Node::SetType(NodeType type)
{
    switch (type) {
        case NodeType::Char:
            this->SetChar('\0');           
            break;
        case NodeType::Short:
            this->SetShort(0);
            break;     
        case NodeType::Int:               
            this->SetInt(0);
            break;
        case NodeType::LongLong:           
            this->SetLongLong(0);
            break;
        case NodeType::String:                
            this->SetString("");
            break;
        default:
            break;
    }
}

const char* Node::GetTypeStr() const
{
    switch (type_) {
        case NodeType::Char:
            return "Char";
        case NodeType::Short:
            return "Short";
        case NodeType::Int:               
            return "Int";
        case NodeType::LongLong:           
            return "LongLong";
        case NodeType::String:                
            return "String";
        default:
            return "Invalid";
    }
}

void Node::CheckDeleteString()
{
    if (type_ == NodeType::String && value_.str != nullptr) {
        delete[] value_.str;
        value_.str = nullptr;
    }
}

void Node::SetChar(char c)
{
    this->CheckDeleteString();

    type_ = NodeType::Char;
    value_.c = c;
}

void Node::SetShort(short s)
{
    this->CheckDeleteString();

    type_ = NodeType::Short;
    value_.s = s;
}

void Node::SetInt(int i)
{
    this->CheckDeleteString();

    type_ = NodeType::Int;
    value_.i = i;
}

void Node::SetLongLong(long long ll)
{
    this->CheckDeleteString();

    type_ = NodeType::LongLong;
    value_.ll = ll;
}

void Node::SetString(const char *str)
{
    char *tmp = new char[strlen(str) + 1];
    strcpy(tmp, str);

    this->CheckDeleteString();

    type_ = NodeType::String;
    value_.str = tmp;
}