#include "HepMC3/GenEvent.h"

#include <TLorentzVector.h>

using namespace HepMC3;


double Phi_Phis(int mode, TLorentzVector q, TLorentzVector p, TLorentzVector mu, TLorentzVector mup, TLorentzVector v){

         //target spin
         TVector3 spin_tar;
         spin_tar.SetXYZ(0., 1., 0.);

         //variables
         TVector3 boost;
         TLorentzVector q_boosted, p_boosted, mu_boosted, mup_boosted, v_boosted, lvp1b2, lvp2b2;
         double sinb2, cosb2;
         double sign;

         //Phi
         double Phi;
         boost = (q+p).BoostVector();
         q_boosted = q;                q_boosted.Boost(-boost);
         p_boosted = p;                p_boosted.Boost(-boost);
         mu_boosted = mu;              mu_boosted.Boost(-boost);
         mup_boosted = mup;            mup_boosted.Boost(-boost);
         v_boosted = v;                v_boosted.Boost(-boost);

         sign = ( ((mu_boosted.Vect()).Cross(mup_boosted.Vect())).Unit() ).Cross( ((q_boosted.Vect()).Cross(v_boosted.Vect())).Unit() ).Dot( (q_boosted.Vect()).Unit() );
         sign /= fabs(sign);

         sinb2 = ( ( ((mu_boosted.Vect()).Cross(mup_boosted.Vect())).Unit() ).Cross( ((q_boosted.Vect()).Cross(v_boosted.Vect())).Unit() ) ).Mag();
         cosb2 = ( ((mu_boosted.Vect()).Cross(mup_boosted.Vect())).Unit() ).Dot( ((q_boosted.Vect()).Cross(v_boosted.Vect())).Unit() );

         Phi = atan2(sign*sinb2, cosb2);
         if( Phi < 0. ) Phi = Phi + 2.*TMath::Pi();

         if( mode == 0 ) return Phi;

         //Phis
         double Phis;

         sign = ( ((mu.Vect()).Cross(mup.Vect())).Unit() ).Cross( (q.Vect()).Cross(spin_tar).Unit() ).Dot( (q.Vect()).Unit() );
         sign /= fabs(sign);

         sinb2 = ( ( ((mu.Vect()).Cross(mup.Vect())).Unit() ).Cross( (q.Vect()).Cross(spin_tar).Unit() ) ).Mag();
         cosb2 = ( ((mu.Vect()).Cross(mup.Vect())).Unit() ).Dot( (q.Vect()).Cross(spin_tar).Unit() );

         Phis = atan2(sign*sinb2, cosb2);
         if( Phis < 0. ) Phis = Phis + 2.*TMath::Pi();

         if( mode == 1 ) return Phis;

         return -1;
}

double Phi(TLorentzVector q, TLorentzVector p, TLorentzVector mu, TLorentzVector mup, TLorentzVector v){
        return Phi_Phis(0, q, p, mu, mup, v);
}

double Phis(TLorentzVector q, TLorentzVector p, TLorentzVector mu, TLorentzVector mup, TLorentzVector v){
        return Phi_Phis(1, q, p, mu, mup, v);
}

TLorentzVector getFourMomentum(std::shared_ptr<const HepMC3::GenParticle> p){
	return TLorentzVector(p->momentum().px(), p->momentum().py(), p->momentum().pz(), p->momentum().e());
}

class DVCSEvent{
	
	public:

	DVCSEvent(const GenEvent& evt){

		//particles
		
		//in electron
		TLorentzVector eIn = getFourMomentum(evt.particles().at(0)); 

		//out electron
		TLorentzVector eOut = getFourMomentum(evt.particles().at(1)); 

		//virtual photon
		TLorentzVector gammaStar = getFourMomentum(evt.particles().at(2)); 

		//in proton
		TLorentzVector pIn = getFourMomentum(evt.particles().at(3)); 

		//out photon
		TLorentzVector gammaOut = getFourMomentum(evt.particles().at(4)); 

		//out proton
		TLorentzVector pOut = getFourMomentum(evt.particles().at(5)); 

		//variables

		m_Q2 = -1 * gammaStar.Mag2();  

		m_xB = m_Q2 / (2 * pIn * gammaStar); 

		m_y = (pIn * gammaStar) / (pIn * eIn); 

		m_t = (pOut - pIn).Mag2();

		m_phi = Phi(gammaStar, pIn, eIn, eOut, gammaOut); 	
	}

	double getXb() const{
		return m_xB;
	}

	double getT() const{
		return m_t;
	}

	double getQ2() const{
		return m_Q2;
	}

	double getY() const{
		return m_y;
	}

	double getPhi() const{
		return m_phi;
	}

	private:

	double m_xB;
	double m_t;
	double m_Q2;
	double m_y;
	double m_phi;
};
