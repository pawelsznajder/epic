#include "HepMC3/GenEvent.h"

#include <TLorentzVector.h>

using namespace HepMC3;

double Phi_Phis_theta(int mode, TLorentzVector q, TLorentzVector p, TLorentzVector lep_k, TLorentzVector lep_kprime, TLorentzVector p_final){

  //target spin
    TVector3 spin_tar;
    spin_tar.SetXYZ(0., 1., 0.);

  //variables
    TVector3 boost, boost_lep, lep_plane, boosted_lep_plane, boosted_had_plane, t_pol_target;
    TLorentzVector q_boosted, p_boosted, lep_k_boosted, lep_kprime_boosted, p_final_boosted, lvp1b2, lvp2b2, lep_k_boosted_CM, p_final_boosted_CM;
    double sinb2, cosb2;
    double sign;

	//Defining frames and boosts
    boost = (q+p).BoostVector();
	 
    q_boosted = q;                      q_boosted.Boost(-boost);
    p_boosted = p;                      p_boosted.Boost(-boost);
    lep_k_boosted = lep_k;              lep_k_boosted.Boost(-boost);
    lep_kprime_boosted = lep_kprime;    lep_kprime_boosted.Boost(-boost);
    p_final_boosted = p_final;          p_final_boosted.Boost(-boost);
	 

    boost_lep = (lep_k+lep_kprime).BoostVector(); //boost from lep CM to lab frame

    lep_k_boosted_CM = lep_k;           lep_k_boosted_CM.Boost(-boost_lep); //go from lab to CM of lep so take -ive
    p_final_boosted_CM = p_final;       p_final_boosted_CM.Boost(-boost_lep);

    lep_plane = ((lep_k.Vect()).Cross(lep_kprime.Vect())).Unit();
    t_pol_target = ((q.Vect()).Cross(spin_tar)).Unit();

    boosted_lep_plane = ((lep_k_boosted.Vect()).Cross(lep_kprime_boosted.Vect())).Unit(); //cross product of k' and k vectors gives vector describing leptonic plane
    boosted_had_plane = ((q_boosted.Vect()).Cross(p_final_boosted.Vect())).Unit();        //cross product of q vector and p' gives vector describing hadronic plane

	//Calculating Phi - angle between leptonic and hadronic plane
    double Phi;

    sign = boosted_lep_plane.Cross(boosted_had_plane).Dot((q_boosted.Vect()).Unit());

    sign /= fabs(sign);

    sinb2 = (boosted_lep_plane.Cross(boosted_had_plane)).Mag();
    cosb2 = boosted_lep_plane.Dot(boosted_had_plane);

    Phi = atan2(sign*sinb2, cosb2);
    if( Phi < 0. ) Phi = Phi + 2.*TMath::Pi(); //negative direction redefined as 180-360 degrees
         if( mode == 0 ) return Phi;

	 //Calculating Phis - angle between leptonic plane and transverse component of target polarisation
    double Phis;

    sign = lep_plane.Cross(t_pol_target).Dot((q.Vect()).Unit());
    sign /= fabs(sign);

    sinb2 = (lep_plane.Cross(t_pol_target)).Mag();
    cosb2 = lep_plane.Dot(t_pol_target);

    Phis = atan2(sign*sinb2, cosb2);
    if( Phis < 0. ) Phis = Phis + 2.*TMath::Pi();

      if( mode == 1 ) return Phis;

	 //Calculating theta - angle between positive decay lepton and p_final in lepton CM
    double cos_theta, theta;
	 
    cos_theta = ((p_final_boosted_CM.Vect()).Unit()).Dot((lep_k_boosted_CM.Vect()).Unit());
    theta = acos(cos_theta);
	 
    if( mode == 2 ) return theta;
	 
return -1;

}

double Phi(TLorentzVector q, TLorentzVector p, TLorentzVector lep_k, TLorentzVector lep_kprime, TLorentzVector p_final){
        return Phi_Phis_theta(0, q, p, lep_k, lep_kprime, p_final);
}

double Phis(TLorentzVector q, TLorentzVector p, TLorentzVector lep_k, TLorentzVector lep_kprime, TLorentzVector p_final){
        return Phi_Phis_theta(1, q, p, lep_k, lep_kprime, p_final);
}

double theta(TLorentzVector q, TLorentzVector p, TLorentzVector lep_k, TLorentzVector lep_kprime, TLorentzVector p_final){
        return Phi_Phis_theta(2, q, p, lep_k, lep_kprime, p_final);
}

TLorentzVector getFourMomentum(std::shared_ptr<const HepMC3::GenParticle> p){
	return TLorentzVector(p->momentum().px(), p->momentum().py(), p->momentum().pz(), p->momentum().e());
}


class TCSEvent{

	public:

	TCSEvent(const GenEvent& evt){

  //Getting four-momentum information from particles
	  /*
    For TCS:
    id = 0 electron beam
    id = 1 scattered electron
    id = 2 real photon (TCS beam)
	id = 3 proton beam	 
	id = 4 virtual photon
    id = 5 recoil proton
	id = 6 decay lepton -
	id = 7 decay lepton +
	*/
	  
		//electron beam
		eIn = getFourMomentum(evt.particles().at(0));

		//proton beam
		pIn = getFourMomentum(evt.particles().at(3));

		//electron scattered
		eOut = getFourMomentum(evt.particles().at(1));

		//real photon
		gamma = getFourMomentum(evt.particles().at(2));

		//proton scattered
		pOut = getFourMomentum(evt.particles().at(5));

		//virtual photon
		gammaStar = getFourMomentum(evt.particles().at(4));

		//decay lepton(-)
		decayLepton2 = getFourMomentum(evt.particles().at(6));

		//decay lepton(+)
		decayLepton1 = getFourMomentum(evt.particles().at(7));

		//Calculating kinematic variables

		m_Q2 = -1 * ( pow(gamma.E(),2) - pow(gamma.P(),2) );  

		m_Qprime2 =  gammaStar.Mag2(); // Q'^2 = -q'^2

		m_xB = m_Q2 / (2 * pIn * gamma); //x_B = Q^2/2pq 

		m_y = (pIn * gamma) / (pIn * eIn); //y=pq/pk=pq/E_e*E_p 

		m_t = (pOut - pIn).Mag2(); //use of proton information - YR shows this is best route

		m_phi = Phi(gamma, pIn, decayLepton1, decayLepton2, pOut);

		m_phis = Phis(gamma, pIn, decayLepton1, decayLepton2, pOut);

		m_theta = theta(gamma, pIn, decayLepton1, decayLepton2, pOut);
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

	double getQprime2() const{
		return m_Qprime2;
	}

	double getY() const{
		return m_y;
	}

	double getPhi() const{
		return m_phi;
	}

	double getPhis() const{
		return m_phis;
	}

	double getTheta() const{
		return m_theta;
	}

	TLorentzVector getPIn() const{
		return pIn;
	}

	TLorentzVector getRecoil() const{
		return pOut;
	}

	TLorentzVector getEIn() const{
		return eIn;
	}

	TLorentzVector getERecoil() const{
		return eOut;
	}

	TLorentzVector getPhoton() const{
		return gamma;
	}
	
	TLorentzVector getVirtual() const{
		return gammaStar;
	}

	TLorentzVector getLep1() const{
		return decayLepton1;
	}

	TLorentzVector getLep2() const{
		return decayLepton2;
	}


	private:

	double m_xB;
	double m_t;
	double m_Q2;
	double m_Qprime2;
	double m_y;
	double m_phi;
	double m_phis;
	double m_theta;
	TLorentzVector pIn;
	TLorentzVector pOut;
	TLorentzVector eIn;
	TLorentzVector eOut;
	TLorentzVector gamma;
	TLorentzVector gammaStar;
	TLorentzVector decayLepton1;
	TLorentzVector decayLepton2;
};
