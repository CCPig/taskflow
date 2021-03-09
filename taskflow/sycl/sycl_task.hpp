#pragma once

#include "sycl_graph.hpp"

/** 
@file sycl_task.hpp
@brief syclTask include file
*/

namespace tf {

// ----------------------------------------------------------------------------
// syclTask Types
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// syclTask 
// ----------------------------------------------------------------------------

/**
@class syclTask

@brief handle to a node of the internal CUDA graph
*/
class syclTask {

  friend class syclFlow;
  friend class syclFlowCapturer;
  friend class syclFlowCapturerBase;

  friend std::ostream& operator << (std::ostream&, const syclTask&);

  public:
    
    /**
    @brief constructs an empty syclTask
    */
    syclTask() = default;

    /**
    @brief copy-constructs a syclTask
    */
    syclTask(const syclTask&) = default;

    /**
    @brief copy-assigns a syclTask
    */
    syclTask& operator = (const syclTask&) = default;

    /**
    @brief adds precedence links from this to other tasks

    @tparam Ts parameter pack

    @param tasks one or multiple tasks

    @return @c *this
    */
    template <typename... Ts>
    syclTask& precede(Ts&&... tasks);
    
    /**
    @brief adds precedence links from other tasks to this

    @tparam Ts parameter pack

    @param tasks one or multiple tasks

    @return @c *this
    */
    template <typename... Ts>
    syclTask& succeed(Ts&&... tasks);
    
    /**
    @brief assigns a name to the task

    @param name a @std_string acceptable string

    @return @c *this
    */
    syclTask& name(const std::string& name);
    
    /**
    @brief queries the name of the task
    */
    const std::string& name() const;

    /**
    @brief queries the number of successors
    */
    size_t num_successors() const;

    /**
    @brief queries if the task is associated with a syclNode
    */
    bool empty() const;

    /**
    @brief dumps the task through an output stream
    
    @tparam T output stream type with insertion operator (<<) defined
    @param ostream an output stream target
    */
    template <typename T>
    void dump(T& ostream) const;

  private:
    
    syclTask(syclNode*);

    syclNode* _node {nullptr};
};

// Constructor
inline syclTask::syclTask(syclNode* node) : _node {node} {
}

// Function: precede
template <typename... Ts>
syclTask& syclTask::precede(Ts&&... tasks) {
  (_node->_precede(tasks._node), ...);
  return *this;
}

// Function: succeed
template <typename... Ts>
syclTask& syclTask::succeed(Ts&&... tasks) {
  (tasks._node->_precede(_node), ...);
  return *this;
}

// Function: empty
inline bool syclTask::empty() const {
  return _node == nullptr;
}

// Function: name
inline syclTask& syclTask::name(const std::string& name) {
  _node->_name = name;
  return *this;
}

// Function: name
inline const std::string& syclTask::name() const {
  return _node->_name;
}

// Function: num_successors
inline size_t syclTask::num_successors() const {
  return _node->_successors.size();
}

// Procedure: dump
template <typename T>
void syclTask::dump(T& os) const {
  os << "syclTask ";
  if(_node->_name.empty()) os << _node;
  else os << _node->_name;
}

// ----------------------------------------------------------------------------
// global ostream
// ----------------------------------------------------------------------------

/**
@brief overload of ostream inserter operator for syclTask
*/
inline std::ostream& operator << (std::ostream& os, const syclTask& ct) {
  ct.dump(os);
  return os;
}

}  // end of namespace tf -----------------------------------------------------



