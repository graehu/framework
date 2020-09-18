#include "rigid_body.h"
#include "colliders/polygon.h"
#include "collision.h"
	 // update::subscribe(update::e_late, [this](float _delta){this->late_update(_delta);});
namespace physics
{
   rigid_body::rigid_body()
   {
      m_current_state.size = 1;
      m_current_state.mass = 1;
      m_current_state.inverseMass = 1.0f / m_current_state.mass;
      m_current_state.position = vec3f(2,0,0);
      m_current_state.momentum = vec3f(0,0,-10);
      m_current_state.orientation.identity();
      m_current_state.angularMomentum = vec3f(0,0,0);
      m_current_state.inertiaTensor = m_current_state.mass * m_current_state.size * m_current_state.size * 1.0f / 6.0f;
      m_current_state.inverseInertiaTensor = 1.0f / m_current_state.inertiaTensor;
      m_current_state.recalculate();
      m_previous_state = m_current_state;
   }

//TODO: this needs to be done differently.
//      the physics engine should be the thing
//      updating bodies and resolving collisions.
   void rigid_body::update(float t, float dt)
   {
      m_previous_state = m_current_state;
      integrate(m_current_state, t, dt);
      m_current_state.forces.clear();
      //if physics are 2d. do this:
      m_current_state.position.k = 0;
      m_current_state.orientation.i = 0;
      m_current_state.orientation.j = 0;
   }
   
   void rigid_body::integrate(state &_state, float t, float dt)
   {
      derivative a = evaluate(_state, t);
      derivative b = evaluate(_state, t, dt*0.5f, a);
      derivative c = evaluate(_state, t, dt*0.5f, b);
      derivative d = evaluate(_state, t, dt, c);
	
      _state.position += 1.0f/6.0f * dt * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);
      _state.momentum += 1.0f/6.0f * dt * (a.force + 2.0f*(b.force + c.force) + d.force);

      _state.orientation += (a.spin + (b.spin + c.spin)*2.0 + d.spin)*(1.0f/6.0f * dt);

      _state.angularMomentum += 1.0f/6.0f * dt * (a.torque + 2.0f*(b.torque + c.torque) + d.torque);

      _state.recalculate();
   }

   void rigid_body::add_collider(collider::collider* _collider)
   {
      m_collider = _collider;
      m_collider->m_physics = this;
   }

   derivative rigid_body::evaluate(const state &_state, float t)
   {
      derivative output;
      output.velocity = _state.velocity;
      output.spin = _state.spin;
      forces(_state, t, output.force, output.torque);
      return output;
   }

   derivative rigid_body::evaluate(state _state, float t, float dt, const derivative &_derivative)
   {
      _state.position += _derivative.velocity * dt;
      _state.momentum += _derivative.force * dt;
      _state.orientation +=  _derivative.spin * dt;
      _state.angularMomentum += _derivative.torque * dt;
      _state.recalculate();
	
      derivative output;
      output.velocity = _state.velocity;
      output.spin = _state.spin;
      forces(_state, t+dt, output.force, output.torque);
      return output;
   }
   
   void rigid_body::forces(const state &_state, float t, vec3f &force, vec3f &torque)
   {
      // attract towards origin
      force.j = -9.8;// * _state.position.j;

      for(int i = 0; i < m_current_state.forces.size(); i++)
	 force = force + m_current_state.forces[i];
   }
   
   void rigid_body::render(iRenderVisitor* _renderer)
   {
      if(m_collider != nullptr)
      {
	 auto poly = dynamic_cast<collider::polygon*>(m_collider);
	 if(poly != nullptr)
	 {
	    std::vector<vec3f> temp = poly->m_vertices;
	    mat4x4f orient;
	    m_current_state.orientation.create_matrix(&orient);
	    for(unsigned int i = 0; i < poly->m_vertices.size(); i++)
	       poly->m_vertices[i] = (orient)*poly->m_vertices[i] + m_current_state.position;
	    _renderer->visit(poly);
	    poly->m_vertices = temp;
	 }
      }
   }

   void rigid_body::add_collision(collision _collision)
   {
      m_collisions.push_back(_collision);
   }

   void rigid_body::resolve_collisions()
   {
      vec3f normMTV;
      vec3f force;
      float angle = 0;
      for(auto col : m_collisions)
      {
	 normMTV = col.m_MTV.normalise();
	 vec3f normVelo = m_current_state.velocity.normalise();
	 angle = normMTV.dot_product2(normVelo);
	 float momentum = m_current_state.momentum.length();
	 force += -normMTV * momentum * angle;
      }
      if(m_collisions.size() > 0)
      {
	 force /= m_collisions.size();
	 apply_impulse(force);
      };
      m_collisions.clear();
   }

   // vec3f rigid_body::collide_sat(polygon* _poly)
   // {
   //    //Do interesting shaz here.
   //    std::vector<vec3f> temp = m_vertices;
   //    for(unsigned int i = 0; i < m_vertices.size(); i++)
   // 	 m_vertices[i] = m_vertices[i] + m_current_state.position;
   //    vec3f MTV = polygon::collide_sat(_poly);

   //    m_forces.push_back(MTV*200);//*1000

   //    m_current_state.position += MTV;
   //    m_vertices = temp;
   //    return MTV;
   // };

   //todo: test this, not sure it works
   state interpolate(const state& a, const state& b, float alpha)
   {
      state state = b;
      state.position = a.position*(1-alpha) + b.position*alpha;
      state.momentum = a.momentum*(1-alpha) + b.momentum*alpha;
      state.orientation = slerp(a.orientation, b.orientation, alpha);
      state.angularMomentum = a.angularMomentum*(1-alpha) + b.angularMomentum*alpha;
      state.recalculate();
      return state;
   }
}
