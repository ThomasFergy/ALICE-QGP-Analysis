/*
*/
{
 gROOT->Reset();
 //
 // Limits for invariant mass histo
 //
 Double_t M_low = 0.45;
 Double_t M_high = 0.55;
 //
 // Get invariant mass histo from file
 //
 TFile *f= new TFile("sum.root");
 f->cd("strangeness_tutorial/genParticles");
 TH1F* h1=(TH1F*)gDirectory->Get("hPtK0ShortGen");
 if(h1 == nullptr) {
   std::cout << "h1 was not read" << std::endl; 
   exit(1);
 }
 h1->Sumw2();
 Double_t norm1 = h1->GetEntries();
 std::cout << norm1 << std::endl;
 //
 f->cd("strangeness_tutorial/kzeroShort");
 TH1F* h2=(TH1F*)gDirectory->Get("hPtK0ShortTrueRec");
 if(h2 == nullptr) {
   std::cout << "h2 was not read" << std::endl; 
   exit(1);
 }
 h2->Sumw2();
 Double_t norm2 = h2->GetEntries();
 std::cout << norm2 << std::endl;
 // Efficiency
 TH1F *heff = new TH1F(*h2);
 heff->Divide(h1);
 heff->Draw();
}
//
// 
//
