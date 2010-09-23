#include "Functionals.h"

class HardSpheresFast_type : public FunctionalInterface {
public:
  HardSpheresFast_type(double R_arg, double kT_arg) : R(R_arg), kT(kT_arg) { have_analytic_grad = false; }
  double transform(double) const {
    return 0;
  }
  double derive(double) const {
    return 0;
  }

  VectorXd transform(const GridDescription &gd, const VectorXd &x) const {
    assert(&gd); // to avoid an unused parameter error
    assert(&x); // to avoid an unused parameter error
    VectorXcd var_0(fft(gd, x));
    VectorXd shell_1(ifft(gd, shell(gd, R).cwise()*var_0));
    VectorXd step_2(ifft(gd, step(gd, R).cwise()*var_0));
    VectorXd xxshell_3(ifft(gd, xxshell(gd, R).cwise()*var_0));
    VectorXd yyshell_4(ifft(gd, yyshell(gd, R).cwise()*var_0));
    VectorXd zzshell_5(ifft(gd, zzshell(gd, R).cwise()*var_0));
    VectorXd xyshell_6(ifft(gd, xyshell(gd, R).cwise()*var_0));
    VectorXd yzshell_7(ifft(gd, yzshell(gd, R).cwise()*var_0));
    VectorXd zxshell_8(ifft(gd, zxshell(gd, R).cwise()*var_0));
    return kT*((-1/(12.56637061435917*(R*R))*shell_1).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()) + kT*((shell_1.cwise().square() - xShellConvolve(R)(gd, x).cwise().square() + (-yShellConvolve(R)(gd, x).cwise().square() - zShellConvolve(R)(gd, x).cwise().square())).cwise()/(12.56637061435917*R*(VectorXd::Ones(gd.NxNyNz) - step_2))) + kT*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(shell_1.cwise()*shell_1.cwise().square() - (3*shell_1).cwise()*(xShellConvolve(R)(gd, x).cwise().square() + yShellConvolve(R)(gd, x).cwise().square() + zShellConvolve(R)(gd, x).cwise().square()) + 9*(xShellConvolve(R)(gd, x).cwise().square().cwise()*xxshell_3 + yShellConvolve(R)(gd, x).cwise().square().cwise()*yyshell_4 + (zShellConvolve(R)(gd, x).cwise().square().cwise()*zzshell_5 + 2*((xShellConvolve(R)(gd, x).cwise()*yShellConvolve(R)(gd, x)).cwise()*xyshell_6 + (yShellConvolve(R)(gd, x).cwise()*zShellConvolve(R)(gd, x)).cwise()*yzshell_7 + (zShellConvolve(R)(gd, x).cwise()*xShellConvolve(R)(gd, x)).cwise()*zxshell_8) + -0.5*(((6*xyshell_6).cwise()*yzshell_7).cwise()*zxshell_8 + xxshell_3.cwise()*(xxshell_3.cwise().square() + 3*xyshell_6.cwise().square() + 3*zxshell_8.cwise().square()) + (yyshell_4.cwise()*(3*xyshell_6.cwise().square() + yyshell_4.cwise().square() + 3*yzshell_7.cwise().square()) + zzshell_5.cwise()*(3*zxshell_8.cwise().square() + 3*yzshell_7.cwise().square() + zzshell_5.cwise().square())))))));
  }
  Functional grad(const Functional &, bool) const {
    assert(false);
    return 0;
  }

  void grad(const GridDescription &gd, const VectorXd &x, const VectorXd &ingrad, VectorXd *outgrad, VectorXd *outpgrad) const {
    assert(&gd); // to avoid an unused parameter error
    assert(&x); // to avoid an unused parameter error
    VectorXcd var_0(fft(gd, x));
    VectorXd shell_1(ifft(gd, shell(gd, R).cwise()*var_0));
    VectorXd step_2(ifft(gd, step(gd, R).cwise()*var_0));
    VectorXcd var_3(fft(gd, -(((-1/(12.56637061435917*(R*R))*shell_1).cwise()*(kT*ingrad)).cwise()/(VectorXd::Ones(gd.NxNyNz) - step_2))));
    VectorXd var_4(ifft(gd, step(gd, R).cwise()*var_3));
    VectorXcd var_5(fft(gd, -1/(12.56637061435917*(R*R))*((VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log().cwise()*(kT*ingrad))));
    VectorXd var_6(ifft(gd, shell(gd, R).cwise()*var_5));
    VectorXcd var_7(fft(gd, shell_1.cwise()*((kT*ingrad).cwise()/(12.56637061435917*R*(VectorXd::Ones(gd.NxNyNz) - step_2)))));
    VectorXd var_8(ifft(gd, shell(gd, R).cwise()*var_7));
    VectorXcd var_9(fft(gd, shell_1.cwise()*(shell_1.cwise()*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))));
    VectorXd var_10(ifft(gd, shell(gd, R).cwise()*var_9));
    VectorXcd var_11(fft(gd, shell_1.cwise().square().cwise()*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))));
    VectorXd var_12(ifft(gd, shell(gd, R).cwise()*var_11));
    VectorXcd var_13(fft(gd, 3*((xShellConvolve(R)(gd, x).cwise().square() + yShellConvolve(R)(gd, x).cwise().square() + zShellConvolve(R)(gd, x).cwise().square()).cwise()*-(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))));
    VectorXd var_14(ifft(gd, shell(gd, R).cwise()*var_13));
    VectorXcd var_15(fft(gd, xShellConvolve(R)(gd, x).cwise().square().cwise()*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))));
    VectorXd var_16(ifft(gd, xxshell(gd, R).cwise()*var_15));
    VectorXd xxshell_17(ifft(gd, xxshell(gd, R).cwise()*var_0));
    VectorXcd var_18(fft(gd, yShellConvolve(R)(gd, x).cwise().square().cwise()*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))));
    VectorXd var_19(ifft(gd, yyshell(gd, R).cwise()*var_18));
    VectorXd yyshell_20(ifft(gd, yyshell(gd, R).cwise()*var_0));
    VectorXcd var_21(fft(gd, zShellConvolve(R)(gd, x).cwise().square().cwise()*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))));
    VectorXd var_22(ifft(gd, zzshell(gd, R).cwise()*var_21));
    VectorXd zzshell_23(ifft(gd, zzshell(gd, R).cwise()*var_0));
    VectorXcd var_24(fft(gd, (xShellConvolve(R)(gd, x).cwise()*yShellConvolve(R)(gd, x)).cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))));
    VectorXd var_25(ifft(gd, xyshell(gd, R).cwise()*var_24));
    VectorXd xyshell_26(ifft(gd, xyshell(gd, R).cwise()*var_0));
    VectorXcd var_27(fft(gd, (yShellConvolve(R)(gd, x).cwise()*zShellConvolve(R)(gd, x)).cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))));
    VectorXd var_28(ifft(gd, yzshell(gd, R).cwise()*var_27));
    VectorXd yzshell_29(ifft(gd, yzshell(gd, R).cwise()*var_0));
    VectorXcd var_30(fft(gd, (zShellConvolve(R)(gd, x).cwise()*xShellConvolve(R)(gd, x)).cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))));
    VectorXd var_31(ifft(gd, zxshell(gd, R).cwise()*var_30));
    VectorXd zxshell_32(ifft(gd, zxshell(gd, R).cwise()*var_0));
    VectorXcd var_33(fft(gd, ((6*xyshell_26).cwise()*yzshell_29).cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))));
    VectorXd var_34(ifft(gd, zxshell(gd, R).cwise()*var_33));
    VectorXcd var_35(fft(gd, (6*xyshell_26).cwise()*(zxshell_32.cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))))));
    VectorXd var_36(ifft(gd, yzshell(gd, R).cwise()*var_35));
    VectorXcd var_37(fft(gd, 6*(yzshell_29.cwise()*(zxshell_32.cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))));
    VectorXd var_38(ifft(gd, xyshell(gd, R).cwise()*var_37));
    VectorXcd var_39(fft(gd, xxshell_17.cwise()*(xxshell_17.cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))))));
    VectorXd var_40(ifft(gd, xxshell(gd, R).cwise()*var_39));
    VectorXcd var_41(fft(gd, xyshell_26.cwise()*(3*(xxshell_17.cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))));
    VectorXd var_42(ifft(gd, xyshell(gd, R).cwise()*var_41));
    VectorXcd var_43(fft(gd, zxshell_32.cwise()*(3*(xxshell_17.cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))));
    VectorXd var_44(ifft(gd, zxshell(gd, R).cwise()*var_43));
    VectorXcd var_45(fft(gd, (xxshell_17.cwise().square() + 3*xyshell_26.cwise().square() + 3*zxshell_32.cwise().square()).cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))));
    VectorXd var_46(ifft(gd, xxshell(gd, R).cwise()*var_45));
    VectorXcd var_47(fft(gd, xyshell_26.cwise()*(3*(yyshell_20.cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))));
    VectorXd var_48(ifft(gd, xyshell(gd, R).cwise()*var_47));
    VectorXcd var_49(fft(gd, yyshell_20.cwise()*(yyshell_20.cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))))));
    VectorXd var_50(ifft(gd, yyshell(gd, R).cwise()*var_49));
    VectorXcd var_51(fft(gd, yzshell_29.cwise()*(3*(yyshell_20.cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))));
    VectorXd var_52(ifft(gd, yzshell(gd, R).cwise()*var_51));
    VectorXcd var_53(fft(gd, (3*xyshell_26.cwise().square() + yyshell_20.cwise().square() + 3*yzshell_29.cwise().square()).cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))));
    VectorXd var_54(ifft(gd, yyshell(gd, R).cwise()*var_53));
    VectorXcd var_55(fft(gd, zxshell_32.cwise()*(3*(zzshell_23.cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))));
    VectorXd var_56(ifft(gd, zxshell(gd, R).cwise()*var_55));
    VectorXcd var_57(fft(gd, yzshell_29.cwise()*(3*(zzshell_23.cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))));
    VectorXd var_58(ifft(gd, yzshell(gd, R).cwise()*var_57));
    VectorXcd var_59(fft(gd, zzshell_23.cwise()*(zzshell_23.cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))))));
    VectorXd var_60(ifft(gd, zzshell(gd, R).cwise()*var_59));
    VectorXcd var_61(fft(gd, (3*zxshell_32.cwise().square() + 3*yzshell_29.cwise().square() + zzshell_23.cwise().square()).cwise()*(0.5*(-9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))));
    VectorXd var_62(ifft(gd, zzshell(gd, R).cwise()*var_61));
    VectorXcd var_63(fft(gd, ((shell_1.cwise()*shell_1.cwise().square() - (3*shell_1).cwise()*(xShellConvolve(R)(gd, x).cwise().square() + yShellConvolve(R)(gd, x).cwise().square() + zShellConvolve(R)(gd, x).cwise().square()) + 9*(xShellConvolve(R)(gd, x).cwise().square().cwise()*xxshell_17 + yShellConvolve(R)(gd, x).cwise().square().cwise()*yyshell_20 + (zShellConvolve(R)(gd, x).cwise().square().cwise()*zzshell_23 + 2*((xShellConvolve(R)(gd, x).cwise()*yShellConvolve(R)(gd, x)).cwise()*xyshell_26 + (yShellConvolve(R)(gd, x).cwise()*zShellConvolve(R)(gd, x)).cwise()*yzshell_29 + (zShellConvolve(R)(gd, x).cwise()*xShellConvolve(R)(gd, x)).cwise()*zxshell_32) + -0.5*(((6*xyshell_26).cwise()*yzshell_29).cwise()*zxshell_32 + xxshell_17.cwise()*(xxshell_17.cwise().square() + 3*xyshell_26.cwise().square() + 3*zxshell_32.cwise().square()) + (yyshell_20.cwise()*(3*xyshell_26.cwise().square() + yyshell_20.cwise().square() + 3*yzshell_29.cwise().square()) + zzshell_23.cwise()*(3*zxshell_32.cwise().square() + 3*yzshell_29.cwise().square() + zzshell_23.cwise().square())))))).cwise()*(kT*ingrad)).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())));
    VectorXd var_64(ifft(gd, step(gd, R).cwise()*var_63));
    VectorXcd var_65(fft(gd, -(((VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(((shell_1.cwise()*shell_1.cwise().square() - (3*shell_1).cwise()*(xShellConvolve(R)(gd, x).cwise().square() + yShellConvolve(R)(gd, x).cwise().square() + zShellConvolve(R)(gd, x).cwise().square()) + 9*(xShellConvolve(R)(gd, x).cwise().square().cwise()*xxshell_17 + yShellConvolve(R)(gd, x).cwise().square().cwise()*yyshell_20 + (zShellConvolve(R)(gd, x).cwise().square().cwise()*zzshell_23 + 2*((xShellConvolve(R)(gd, x).cwise()*yShellConvolve(R)(gd, x)).cwise()*xyshell_26 + (yShellConvolve(R)(gd, x).cwise()*zShellConvolve(R)(gd, x)).cwise()*yzshell_29 + (zShellConvolve(R)(gd, x).cwise()*xShellConvolve(R)(gd, x)).cwise()*zxshell_32) + -0.5*(((6*xyshell_26).cwise()*yzshell_29).cwise()*zxshell_32 + xxshell_17.cwise()*(xxshell_17.cwise().square() + 3*xyshell_26.cwise().square() + 3*zxshell_32.cwise().square()) + (yyshell_20.cwise()*(3*xyshell_26.cwise().square() + yyshell_20.cwise().square() + 3*yzshell_29.cwise().square()) + zzshell_23.cwise()*(3*zxshell_32.cwise().square() + 3*yzshell_29.cwise().square() + zzshell_23.cwise().square())))))).cwise()*(kT*ingrad)).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square()))).cwise()/(VectorXd::Ones(gd.NxNyNz) - step_2))));
    VectorXd var_66(ifft(gd, step(gd, R).cwise()*var_65));
    VectorXcd var_67(fft(gd, -((VectorXd::Ones(gd.NxNyNz) - step_2).cwise()*((VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log().cwise()*(((shell_1.cwise()*shell_1.cwise().square() - (3*shell_1).cwise()*(xShellConvolve(R)(gd, x).cwise().square() + yShellConvolve(R)(gd, x).cwise().square() + zShellConvolve(R)(gd, x).cwise().square()) + 9*(xShellConvolve(R)(gd, x).cwise().square().cwise()*xxshell_17 + yShellConvolve(R)(gd, x).cwise().square().cwise()*yyshell_20 + (zShellConvolve(R)(gd, x).cwise().square().cwise()*zzshell_23 + 2*((xShellConvolve(R)(gd, x).cwise()*yShellConvolve(R)(gd, x)).cwise()*xyshell_26 + (yShellConvolve(R)(gd, x).cwise()*zShellConvolve(R)(gd, x)).cwise()*yzshell_29 + (zShellConvolve(R)(gd, x).cwise()*xShellConvolve(R)(gd, x)).cwise()*zxshell_32) + -0.5*(((6*xyshell_26).cwise()*yzshell_29).cwise()*zxshell_32 + xxshell_17.cwise()*(xxshell_17.cwise().square() + 3*xyshell_26.cwise().square() + 3*zxshell_32.cwise().square()) + (yyshell_20.cwise()*(3*xyshell_26.cwise().square() + yyshell_20.cwise().square() + 3*yzshell_29.cwise().square()) + zzshell_23.cwise()*(3*zxshell_32.cwise().square() + 3*yzshell_29.cwise().square() + zzshell_23.cwise().square())))))).cwise()*(kT*ingrad)).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square()))))));
    VectorXd var_68(ifft(gd, step(gd, R).cwise()*var_67));
    VectorXcd var_69(fft(gd, -((VectorXd::Ones(gd.NxNyNz) - step_2).cwise()*((113.0973355292326*step_2.cwise().square()).cwise()*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()*((shell_1.cwise()*shell_1.cwise().square() - (3*shell_1).cwise()*(xShellConvolve(R)(gd, x).cwise().square() + yShellConvolve(R)(gd, x).cwise().square() + zShellConvolve(R)(gd, x).cwise().square()) + 9*(xShellConvolve(R)(gd, x).cwise().square().cwise()*xxshell_17 + yShellConvolve(R)(gd, x).cwise().square().cwise()*yyshell_20 + (zShellConvolve(R)(gd, x).cwise().square().cwise()*zzshell_23 + 2*((xShellConvolve(R)(gd, x).cwise()*yShellConvolve(R)(gd, x)).cwise()*xyshell_26 + (yShellConvolve(R)(gd, x).cwise()*zShellConvolve(R)(gd, x)).cwise()*yzshell_29 + (zShellConvolve(R)(gd, x).cwise()*xShellConvolve(R)(gd, x)).cwise()*zxshell_32) + -0.5*(((6*xyshell_26).cwise()*yzshell_29).cwise()*zxshell_32 + xxshell_17.cwise()*(xxshell_17.cwise().square() + 3*xyshell_26.cwise().square() + 3*zxshell_32.cwise().square()) + (yyshell_20.cwise()*(3*xyshell_26.cwise().square() + yyshell_20.cwise().square() + 3*yzshell_29.cwise().square()) + zzshell_23.cwise()*(3*zxshell_32.cwise().square() + 3*yzshell_29.cwise().square() + zzshell_23.cwise().square())))))).cwise()*(kT*ingrad))).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square()).cwise().square())))));
    VectorXd var_70(ifft(gd, step(gd, R).cwise()*var_69));
    VectorXcd var_71(fft(gd, step_2.cwise()*(113.0973355292326*((VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()*((shell_1.cwise()*shell_1.cwise().square() - (3*shell_1).cwise()*(xShellConvolve(R)(gd, x).cwise().square() + yShellConvolve(R)(gd, x).cwise().square() + zShellConvolve(R)(gd, x).cwise().square()) + 9*(xShellConvolve(R)(gd, x).cwise().square().cwise()*xxshell_17 + yShellConvolve(R)(gd, x).cwise().square().cwise()*yyshell_20 + (zShellConvolve(R)(gd, x).cwise().square().cwise()*zzshell_23 + 2*((xShellConvolve(R)(gd, x).cwise()*yShellConvolve(R)(gd, x)).cwise()*xyshell_26 + (yShellConvolve(R)(gd, x).cwise()*zShellConvolve(R)(gd, x)).cwise()*yzshell_29 + (zShellConvolve(R)(gd, x).cwise()*xShellConvolve(R)(gd, x)).cwise()*zxshell_32) + -0.5*(((6*xyshell_26).cwise()*yzshell_29).cwise()*zxshell_32 + xxshell_17.cwise()*(xxshell_17.cwise().square() + 3*xyshell_26.cwise().square() + 3*zxshell_32.cwise().square()) + (yyshell_20.cwise()*(3*xyshell_26.cwise().square() + yyshell_20.cwise().square() + 3*yzshell_29.cwise().square()) + zzshell_23.cwise()*(3*zxshell_32.cwise().square() + 3*yzshell_29.cwise().square() + zzshell_23.cwise().square())))))).cwise()*(kT*ingrad))).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square()).cwise().square())))));
    VectorXd var_72(ifft(gd, step(gd, R).cwise()*var_71));
    VectorXcd var_73(fft(gd, -(12.56637061435917*R*(((shell_1.cwise().square() - xShellConvolve(R)(gd, x).cwise().square() + (-yShellConvolve(R)(gd, x).cwise().square() - zShellConvolve(R)(gd, x).cwise().square())).cwise()*(kT*ingrad)).cwise()/(12.56637061435917*R*(VectorXd::Ones(gd.NxNyNz) - step_2)).cwise().square()))));
    VectorXd var_74(ifft(gd, step(gd, R).cwise()*var_73));
    *outgrad += var_4 + var_6 + (2*var_8 + (2*xShellConvolve(R)(gd, -(xShellConvolve(R)(gd, x).cwise()*-((kT*ingrad).cwise()/(12.56637061435917*R*(VectorXd::Ones(gd.NxNyNz) - step_2))))) + (2*yShellConvolve(R)(gd, -(yShellConvolve(R)(gd, x).cwise()*-((kT*ingrad).cwise()/(12.56637061435917*R*(VectorXd::Ones(gd.NxNyNz) - step_2))))) + (2*zShellConvolve(R)(gd, -(zShellConvolve(R)(gd, x).cwise()*-((kT*ingrad).cwise()/(12.56637061435917*R*(VectorXd::Ones(gd.NxNyNz) - step_2))))) + (2*var_10 + var_12 + (2*xShellConvolve(R)(gd, -(xShellConvolve(R)(gd, x).cwise()*((3*shell_1).cwise()*-(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))) + (2*yShellConvolve(R)(gd, -(yShellConvolve(R)(gd, x).cwise()*((3*shell_1).cwise()*-(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))) + 2*zShellConvolve(R)(gd, -(zShellConvolve(R)(gd, x).cwise()*((3*shell_1).cwise()*-(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))) + (var_14 + (var_16 + (2*xShellConvolve(R)(gd, -(xShellConvolve(R)(gd, x).cwise()*(xxshell_17.cwise()*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))))) + var_19 + (2*yShellConvolve(R)(gd, -(yShellConvolve(R)(gd, x).cwise()*(yyshell_20.cwise()*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))))) + var_22 + (2*zShellConvolve(R)(gd, -(zShellConvolve(R)(gd, x).cwise()*(zzshell_23.cwise()*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))))) + (var_25 + yShellConvolve(R)(gd, -(xShellConvolve(R)(gd, x).cwise()*(xyshell_26.cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))) + (xShellConvolve(R)(gd, -(yShellConvolve(R)(gd, x).cwise()*(xyshell_26.cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))) + var_28 + (zShellConvolve(R)(gd, -(yShellConvolve(R)(gd, x).cwise()*(yzshell_29.cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))) + yShellConvolve(R)(gd, -(zShellConvolve(R)(gd, x).cwise()*(yzshell_29.cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))) + (var_31 + xShellConvolve(R)(gd, -(zShellConvolve(R)(gd, x).cwise()*(zxshell_32.cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))) + (zShellConvolve(R)(gd, -(xShellConvolve(R)(gd, x).cwise()*(zxshell_32.cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))) + (var_34 + (var_36 + (var_38 + 2*var_40 + (2*var_42 + (2*var_44 + (var_46 + (2*var_48 + 2*var_50 + (2*var_52 + (var_54 + (2*var_56 + (2*var_58 + 2*var_60 + (var_62 + var_64 + (var_66 + 2*var_68 - (2*var_70 + 2*var_72)))))))))))))))))))))))))) - var_74)))));
    if (outpgrad) *outpgrad += var_4 + var_6 + (2*var_8 + (2*xShellConvolve(R)(gd, -(xShellConvolve(R)(gd, x).cwise()*-((kT*ingrad).cwise()/(12.56637061435917*R*(VectorXd::Ones(gd.NxNyNz) - step_2))))) + (2*yShellConvolve(R)(gd, -(yShellConvolve(R)(gd, x).cwise()*-((kT*ingrad).cwise()/(12.56637061435917*R*(VectorXd::Ones(gd.NxNyNz) - step_2))))) + (2*zShellConvolve(R)(gd, -(zShellConvolve(R)(gd, x).cwise()*-((kT*ingrad).cwise()/(12.56637061435917*R*(VectorXd::Ones(gd.NxNyNz) - step_2))))) + (2*var_10 + var_12 + (2*xShellConvolve(R)(gd, -(xShellConvolve(R)(gd, x).cwise()*((3*shell_1).cwise()*-(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))) + (2*yShellConvolve(R)(gd, -(yShellConvolve(R)(gd, x).cwise()*((3*shell_1).cwise()*-(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))) + 2*zShellConvolve(R)(gd, -(zShellConvolve(R)(gd, x).cwise()*((3*shell_1).cwise()*-(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))) + (var_14 + (var_16 + (2*xShellConvolve(R)(gd, -(xShellConvolve(R)(gd, x).cwise()*(xxshell_17.cwise()*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))))) + var_19 + (2*yShellConvolve(R)(gd, -(yShellConvolve(R)(gd, x).cwise()*(yyshell_20.cwise()*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))))) + var_22 + (2*zShellConvolve(R)(gd, -(zShellConvolve(R)(gd, x).cwise()*(zzshell_23.cwise()*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad)))))) + (var_25 + yShellConvolve(R)(gd, -(xShellConvolve(R)(gd, x).cwise()*(xyshell_26.cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))) + (xShellConvolve(R)(gd, -(yShellConvolve(R)(gd, x).cwise()*(xyshell_26.cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))) + var_28 + (zShellConvolve(R)(gd, -(yShellConvolve(R)(gd, x).cwise()*(yzshell_29.cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))) + yShellConvolve(R)(gd, -(zShellConvolve(R)(gd, x).cwise()*(yzshell_29.cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))) + (var_31 + xShellConvolve(R)(gd, -(zShellConvolve(R)(gd, x).cwise()*(zxshell_32.cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))) + (zShellConvolve(R)(gd, -(xShellConvolve(R)(gd, x).cwise()*(zxshell_32.cwise()*(2*(9*(((step_2 + (VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square().cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().log()).cwise()/((113.0973355292326*step_2.cwise().square()).cwise()*(VectorXd::Ones(gd.NxNyNz) - step_2).cwise().square())).cwise()*(kT*ingrad))))))) + (var_34 + (var_36 + (var_38 + 2*var_40 + (2*var_42 + (2*var_44 + (var_46 + (2*var_48 + 2*var_50 + (2*var_52 + (var_54 + (2*var_56 + (2*var_58 + 2*var_60 + (var_62 + var_64 + (var_66 + 2*var_68 - (2*var_70 + 2*var_72)))))))))))))))))))))))))) - var_74)))));
  }

  Expression printme(const Expression &) const {
    return Expression("Can't print optimized Functionals");
  }
private:
  double R;
  double kT;
};

Functional HardSpheresFast(double R, double kT) {
  return Functional(new HardSpheresFast_type(R, kT), "HardSpheresFast");
}
