#include "datamodel/MCParticleCollection.h"

MCParticleCollection::MCParticleCollection() : m_collectionID(0), m_data(new MCParticleVector() ){
}

const MCParticleHandle& MCParticleCollection::get(int index) const{
  return m_handles[index];
}

MCParticleHandle MCParticleCollection::create() {
  m_data->emplace_back(MCParticle());
  int index = m_data->size()-1;
  // std::cout<<"creating handle: "<<index<<"/"<<m_collectionID<<std::endl;
  m_handles.emplace_back(MCParticleHandle(index,m_collectionID, m_data));

  return m_handles.back();
}

MCParticleHandle MCParticleCollection::insert(const MCParticleHandle& origin) {
  m_data->emplace_back(origin.read());
  int index = m_data->size()-1;
  m_handles.emplace_back(MCParticleHandle(index,m_collectionID, m_data));

  return m_handles.back();
}  

void MCParticleCollection::clear(){
  m_data->clear();
  m_handles.clear();

}

void MCParticleCollection::prepareForWrite(const albers::Registry* registry){
  for(auto& data : *m_data){
     data.StartVertex.prepareForWrite(registry);
    data.EndVertex.prepareForWrite(registry);
  }
}

void MCParticleCollection::prepareAfterRead(albers::Registry* registry){
  m_handles.clear();
  int index = 0;
  // fix. otherwise, m_collectionID == 0..
  m_collectionID = registry->getIDFromPODAddress( _getBuffer() );
  for (auto& data : *m_data){
    data.StartVertex.prepareAfterRead(registry);
data.EndVertex.prepareAfterRead(registry);

    m_handles.emplace_back(MCParticleHandle(index,m_collectionID, m_data));
    ++index;
  }
}


void MCParticleCollection::setPODsAddress(const void* address){
  m_data = (MCParticleVector*)address;
}


const MCParticleHandle MCParticleCollectionIterator::operator* () const {
  return m_collection->get(m_index);
}

//std::vector<std::pair<std::string,albers::CollectionBase*>>& referenceCollections() {
//}


void MCParticleCollection::print() const {
  std::cout<<"collection "<<m_collectionID
           <<", buf "<<m_data
           <<", nhandles "<<m_handles.size()<<std::endl;
}

