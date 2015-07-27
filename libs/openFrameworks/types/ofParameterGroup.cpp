#include "ofUtils.h"
#include "ofParameter.h"

ofParameterGroup::ofParameterGroup()
:obj(new Value)
{

}

void ofParameterGroup::add(ofAbstractParameter & parameter){
	shared_ptr<ofAbstractParameter> param = parameter.newReference();
	obj->parameters.push_back(param);
	obj->parametersIndex[param->getEscapedName()] = obj->parameters.size()-1;
	param->setParent(*this);
}

void ofParameterGroup::clear(){
	obj->parameters.clear();
	obj->parametersIndex.clear();
}

ofParameter<bool> ofParameterGroup::getBool(const string& name) const	{
	return get<bool>(name);
}

ofParameter<int> ofParameterGroup::getInt(const string& name) const{
	return get<int>(name);
}

ofParameter<float> ofParameterGroup::getFloat(const string& name) const{
	return get<float>(name);
}

ofParameter<char> ofParameterGroup::getChar(const string& name) const{
	return get<char>(name);
}

ofParameter<string> ofParameterGroup::getString(const string& name) const{
	return get<string>(name);
}

ofParameter<ofPoint> ofParameterGroup::getPoint(const string& name) const{
	return get<ofPoint>(name);
}

ofParameter<ofVec2f> ofParameterGroup::getVec2f(const string& name) const{
	return get<ofVec2f>(name);
}

ofParameter<ofVec3f> ofParameterGroup::getVec3f(const string& name) const{
	return get<ofVec3f>(name);
}

ofParameter<ofVec4f> ofParameterGroup::getVec4f(const string& name) const{
	return get<ofVec4f>(name);
}

ofParameter<ofColor> ofParameterGroup::getColor(const string& name) const{
	return get<ofColor>(name);
}

ofParameter<ofShortColor> ofParameterGroup::getShortColor(const string& name) const{
	return get<ofShortColor>(name);
}

ofParameter<ofFloatColor> ofParameterGroup::getFloatColor(const string& name) const{
	return get<ofFloatColor>(name);
}

ofParameterGroup ofParameterGroup::getGroup(string name) const{
	return static_cast<ofParameterGroup& >(get(name));
}

ofParameter<bool> ofParameterGroup::getBool(int pos) const{
	return get<bool>(pos);
}

ofParameter<int> ofParameterGroup::getInt(int pos) const{
	return get<int>(pos);
}

ofParameter<float> ofParameterGroup::getFloat(int pos) const{
	return get<float>(pos);
}

ofParameter<char> ofParameterGroup::getChar(int pos) const{
	return get<char>(pos);
}

ofParameter<string> ofParameterGroup::getString(int pos) const{
	return get<string>(pos);
}

ofParameter<ofPoint> ofParameterGroup::getPoint(int pos)	 const{
	return get<ofPoint>(pos);
}

ofParameter<ofVec2f> ofParameterGroup::getVec2f(int pos) const{
	return get<ofVec2f>(pos);
}

ofParameter<ofVec3f> ofParameterGroup::getVec3f(int pos) const{
	return get<ofVec3f>(pos);
}

ofParameter<ofVec4f> ofParameterGroup::getVec4f(int pos) const{
	return get<ofVec4f>(pos);
}

ofParameter<ofColor> ofParameterGroup::getColor(int pos) const{
	return get<ofColor>(pos);
}

ofParameter<ofShortColor> ofParameterGroup::getShortColor(int pos) const{
	return get<ofShortColor>(pos);
}

ofParameter<ofFloatColor> ofParameterGroup::getFloatColor(int pos) const{
	return get<ofFloatColor>(pos);
}


ofParameterGroup ofParameterGroup::getGroup(int pos) const{
	if(pos>=size()){
		return ofParameterGroup();
	}else{
		if(getType(pos)==typeid(ofParameterGroup).name()){
			return *static_cast<ofParameterGroup* >(obj->parameters[pos].get());
		}else{
			ofLogError("ofParameterGroup") << "get(): bad type for pos " << pos << ", returning empty group";
			return ofParameterGroup();
		}
	}
}


int ofParameterGroup::size() const{
	return obj->parameters.size();
}

string ofParameterGroup::getName(int position) const{
	if(position>=size()){
		return "";
	}else{
		return obj->parameters[position]->getName();
	}
}

string ofParameterGroup::getType(int position) const{
	if(position>=size()) return "";
	else return obj->parameters[position]->type();
}


int ofParameterGroup::getPosition(const string& name) const{
	if(obj->parametersIndex.find(escape(name))!=obj->parametersIndex.end())
		return obj->parametersIndex.find(escape(name))->second;
	return -1;
}

string ofParameterGroup::getName() const{
	return obj->name;
}

void ofParameterGroup::setName(const string & name){
	obj->name = name;
}

string ofParameterGroup::getEscapedName() const{
	if(getName()==""){
		return "group";
	}else{
		return ofAbstractParameter::getEscapedName();
	}
}

string ofParameterGroup::toString() const{
	stringstream out;
	out << *this;
	return out.str();
}

void ofParameterGroup::fromString(const string & name){
	ofLogWarning() << "ofParameterGroup doesn't implement fromString yet";
}


ofAbstractParameter & ofParameterGroup::get(const string& name) const{
	map<string,int>::const_iterator it = obj->parametersIndex.find(escape(name));
	int index = it->second;
	return get(index);
}

ofAbstractParameter & ofParameterGroup::get(int pos) const{
	return *obj->parameters[pos];
}


ofAbstractParameter & ofParameterGroup::operator[](const string& name) const{
	return get(name);
}

ofAbstractParameter & ofParameterGroup::operator[](int pos) const{
	return get(pos);
}

ostream& operator<<(ostream& os, const ofParameterGroup& group) {
	std::streamsize width = os.width();
	for(int i=0;i<group.size();i++){
		if(group.getType(i)==typeid(ofParameterGroup).name()){
			os << group.getName(i) << ":" << endl;
			os << setw(width+4);
			os << group.getGroup(i);
		}else{
			os << group.getName(i) << ":" << group.get(i) << endl;
			os << setw(width);
		}
	}
	return os;
}

bool ofParameterGroup::contains(const string& name){
	return obj->parametersIndex.find(escape(name))!=obj->parametersIndex.end();
}

void ofParameterGroup::Value::notifyParameterChanged(ofAbstractParameter & param){
	ofNotifyEvent(parameterChangedE,param);
	parents.erase(std::remove_if(parents.begin(),parents.end(),[&param](weak_ptr<Value> p){
		auto parent = p.lock();
		if(parent) parent->notifyParameterChanged(param);
		return !parent;
	}),parents.end());
}

const ofParameterGroup ofParameterGroup::getFirstParent() const{
	auto first = std::find_if(obj->parents.begin(),obj->parents.end(),[](weak_ptr<Value> p){return p.lock()!=nullptr;});
	if(first!=obj->parents.end()){
		return first->lock();
	}else{
		return shared_ptr<Value>(nullptr);
	}
}

ofEvent<ofAbstractParameter> & ofParameterGroup::parameterChangedE(){
	return obj->parameterChangedE;
}

ofAbstractParameter & ofParameterGroup::back(){
	return *obj->parameters.back();
}

ofAbstractParameter & ofParameterGroup::front(){
	return *obj->parameters.front();
}

const ofAbstractParameter & ofParameterGroup::back() const{
	return *obj->parameters.back();
}

const ofAbstractParameter & ofParameterGroup::front() const{
	return *obj->parameters.front();
}

void ofParameterGroup::setSerializable(bool _serializable){
	obj->serializable = _serializable;
}

bool ofParameterGroup::isSerializable() const{
	return obj->serializable;
}

bool ofParameterGroup::isReadOnly() const{
	return false;
}

shared_ptr<ofAbstractParameter> ofParameterGroup::newReference() const{
	return shared_ptr<ofAbstractParameter>(new ofParameterGroup(*this));
}

void ofParameterGroup::setParent(ofParameterGroup & parent){
	obj->parents.emplace_back(parent.obj);
}

ofParameterGroup::operator bool() const{
	return obj != nullptr;
}

vector<shared_ptr<ofAbstractParameter> >::iterator ofParameterGroup::begin(){
	return obj->parameters.begin();
}

vector<shared_ptr<ofAbstractParameter> >::iterator ofParameterGroup::end(){
	return obj->parameters.end();
}

vector<shared_ptr<ofAbstractParameter> >::const_iterator ofParameterGroup::begin() const{
	return obj->parameters.begin();
}

vector<shared_ptr<ofAbstractParameter> >::const_iterator ofParameterGroup::end() const{
	return obj->parameters.end();
}

vector<shared_ptr<ofAbstractParameter> >::reverse_iterator ofParameterGroup::rbegin(){
	return obj->parameters.rbegin();
}

vector<shared_ptr<ofAbstractParameter> >::reverse_iterator ofParameterGroup::rend(){
	return obj->parameters.rend();
}

vector<shared_ptr<ofAbstractParameter> >::const_reverse_iterator ofParameterGroup::rbegin() const{
	return obj->parameters.rbegin();
}

vector<shared_ptr<ofAbstractParameter> >::const_reverse_iterator ofParameterGroup::rend() const{
	return obj->parameters.rend();
}

