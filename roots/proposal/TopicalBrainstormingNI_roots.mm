<map version="0.9.0">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node COLOR="#000000" CREATED="1330433342276" ID="ID_1724700733" MODIFIED="1331889800784" STYLE="fork" TEXT="Neutron imaging for roots and soil">
<edge WIDTH="thin"/>
<font NAME="SansSerif" SIZE="20"/>
<hook NAME="accessories/plugins/HierarchicalIcons.properties"/>
<hook NAME="accessories/plugins/AutomaticLayout.properties"/>
<node COLOR="#0033ff" CREATED="1330433379673" FOLDED="true" ID="ID_806119907" MODIFIED="1331825510666" POSITION="right" TEXT="Radiography of thin slabs">
<edge STYLE="sharp_bezier" WIDTH="8"/>
<font NAME="SansSerif" SIZE="18"/>
<node COLOR="#00b439" CREATED="1330433662291" FOLDED="true" ID="ID_1782248191" MODIFIED="1331825243819" TEXT="Time series in 2D">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
<node COLOR="#990000" CREATED="1331806924555" ID="ID_336709263" MODIFIED="1331806952263" TEXT="Analyze images in the time directions">
<font NAME="SansSerif" SIZE="14"/>
</node>
<node COLOR="#990000" CREATED="1331806955914" ID="ID_364182960" MODIFIED="1331806978696" TEXT="Requirement accurate stitching">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1331807175610" ID="ID_1072647747" MODIFIED="1331807186616" TEXT="How is it done today"/>
<node COLOR="#111111" CREATED="1331806981410" ID="ID_436874364" MODIFIED="1331807011133" TEXT="Define markers that supprt the registration task"/>
<node COLOR="#111111" CREATED="1331807012074" ID="ID_695493041" MODIFIED="1331807037713" TEXT="Automtize a robust stitching method">
<node COLOR="#111111" CREATED="1331807065979" ID="ID_665617498" MODIFIED="1331807162744" TEXT="first step match images from the same spatial frame"/>
</node>
</node>
</node>
<node COLOR="#00b439" CREATED="1330433678880" FOLDED="true" ID="ID_1816246505" MODIFIED="1331825244740" TEXT="Scattering correction">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
<node COLOR="#990000" CREATED="1330434385682" ID="ID_605574333" MODIFIED="1331805616466" TEXT="Investigate significance of the correction">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1330434495847" ID="ID_1120270047" MODIFIED="1331805616466" TEXT="On root analysis"/>
<node COLOR="#111111" CREATED="1330434517995" ID="ID_1440970182" MODIFIED="1331805616466" TEXT="Water content estimate"/>
</node>
</node>
<node COLOR="#00b439" CREATED="1330433764803" FOLDED="true" ID="ID_307636044" MODIFIED="1331825245957" TEXT="Image processing">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
<node COLOR="#990000" CREATED="1330433773202" ID="ID_145193509" MODIFIED="1331805616467" TEXT="Root tracker 2D">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1330433841030" ID="ID_1141602234" MODIFIED="1331805616467" TEXT="Pre-processing"/>
<node COLOR="#111111" CREATED="1330433789284" ID="ID_974178586" MODIFIED="1331805616467" TEXT="Improve detection performance"/>
<node COLOR="#111111" CREATED="1330433801035" ID="ID_1586434873" MODIFIED="1331805616467" TEXT="Reduce/optimize number of controlling parameter"/>
<node COLOR="#111111" CREATED="1330434116790" ID="ID_1129396547" MODIFIED="1331805616467" TEXT="Verification against phantom data">
<node COLOR="#111111" CREATED="1330434142907" ID="ID_191814525" MODIFIED="1331805616468" TEXT="Physical phantom"/>
<node COLOR="#111111" CREATED="1330434165646" ID="ID_134787729" MODIFIED="1331805616468" TEXT="Model phantom"/>
</node>
<node COLOR="#111111" CREATED="1330434180617" ID="ID_925663821" MODIFIED="1331805616468" TEXT="Find sensitivity of the method under different conditions"/>
<node COLOR="#111111" CREATED="1330434265568" ID="ID_364977746" MODIFIED="1331806314824" TEXT="Quantify root morphology">
<node COLOR="#111111" CREATED="1330435482234" ID="ID_1046474299" MODIFIED="1331805616468" TEXT="What are relevant metrics?"/>
</node>
<node COLOR="#111111" CREATED="1331806317185" ID="ID_411814127" MODIFIED="1331806352308" TEXT="Define a discrete root tree model">
<node COLOR="#111111" CREATED="1331806353604" ID="ID_1676726066" MODIFIED="1331806369116" TEXT="Is it useful?"/>
</node>
</node>
<node COLOR="#990000" CREATED="1331805446025" ID="ID_1074350095" MODIFIED="1331805616465" TEXT="Improving signal to noise by dark current modelling">
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
</node>
<node COLOR="#0033ff" CREATED="1330433390791" ID="ID_1873966203" MODIFIED="1331890664384" POSITION="right" TEXT="Tomography">
<edge STYLE="sharp_bezier" WIDTH="8"/>
<font NAME="SansSerif" SIZE="18"/>
<node COLOR="#00b439" CREATED="1330433400165" ID="ID_1311911355" MODIFIED="1331823383713" TEXT="Image processing">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
<node COLOR="#990000" CREATED="1330433747446" FOLDED="true" ID="ID_566122608" MODIFIED="1331825223192" TEXT="Root tracker 3D">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1330434556394" ID="ID_1194770428" MODIFIED="1331805616470" TEXT="Find improvements on the existing procedure"/>
<node COLOR="#111111" CREATED="1330433897586" ID="ID_666439578" MODIFIED="1331805616470" TEXT="Data enhancement">
<node COLOR="#111111" CREATED="1331805110830" ID="ID_1458368020" MODIFIED="1331805616470" TEXT="Matched filters response mixing"/>
<node COLOR="#111111" CREATED="1331805129366" ID="ID_1443459876" MODIFIED="1331805616470" TEXT="ISS filtering"/>
<node COLOR="#111111" CREATED="1331805139341" ID="ID_451193770" MODIFIED="1331805616470" TEXT="Wavelet filtering"/>
</node>
<node COLOR="#111111" CREATED="1330435364137" ID="ID_1146262974" MODIFIED="1331805616470" TEXT="Segmentation/tracking">
<node COLOR="#111111" CREATED="1330435379936" ID="ID_687825777" MODIFIED="1331805616470" TEXT="Current method is threshold based">
<node COLOR="#111111" CREATED="1331805215660" ID="ID_513208409" MODIFIED="1331805616471" TEXT="Test scale spaces"/>
<node COLOR="#111111" CREATED="1331805232405" ID="ID_781495656" MODIFIED="1331805616471" TEXT="Gradient guided thresholding"/>
<node COLOR="#111111" CREATED="1331805303115" ID="ID_692383502" MODIFIED="1331805616471" TEXT="The current method requires continuous root structure"/>
</node>
<node COLOR="#111111" CREATED="1330435413329" ID="ID_32440673" MODIFIED="1331805616471" TEXT="Try tracking methods">
<node COLOR="#111111" CREATED="1331805357243" ID="ID_1795445501" MODIFIED="1331805616471" TEXT="Deformable trees"/>
<node COLOR="#111111" CREATED="1331805364237" ID="ID_1385707722" MODIFIED="1331805616471" TEXT="Orientation guided tracking"/>
</node>
</node>
<node COLOR="#111111" CREATED="1330434616133" ID="ID_994735473" MODIFIED="1331805616472" TEXT="Sensitivity evaluation">
<node COLOR="#111111" CREATED="1330435270215" ID="ID_1823184251" MODIFIED="1331805616472" TEXT="Simulation with break-down curves">
<node COLOR="#111111" CREATED="1331804795612" ID="ID_888833053" MODIFIED="1331805616472" TEXT="find the sensitivity limits"/>
<node COLOR="#111111" CREATED="1331804829981" ID="ID_1940443369" MODIFIED="1331805616472" TEXT="Noise"/>
<node COLOR="#111111" CREATED="1331804834685" ID="ID_549977533" MODIFIED="1331805616472" TEXT="Contrast"/>
</node>
<node COLOR="#111111" CREATED="1330435286854" ID="ID_1963220737" MODIFIED="1331805616472" TEXT="Relate results to experimental data">
<node COLOR="#111111" CREATED="1330435307701" ID="ID_1428683689" MODIFIED="1331805616473" TEXT="Which data is sufficient for a reliable analysis"/>
</node>
</node>
</node>
<node COLOR="#990000" CREATED="1331825188229" ID="ID_1516084793" MODIFIED="1331825199069" TEXT="Effect of different water types">
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#00b439" CREATED="1330433525793" ID="ID_1105275754" MODIFIED="1331823538934" TEXT="Reconstruction methods">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
<node COLOR="#990000" CREATED="1330433560829" FOLDED="true" ID="ID_682465974" MODIFIED="1331825224719" TEXT="CT with complete data set">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1331808205608" ID="ID_627892924" MODIFIED="1331808232649" TEXT="What is the thinnest root that can be resolved"/>
<node COLOR="#111111" CREATED="1331808233392" ID="ID_1748479301" MODIFIED="1331808248233" TEXT="Which thickness is relevant?"/>
</node>
<node COLOR="#990000" CREATED="1330433576806" FOLDED="true" ID="ID_29700871" MODIFIED="1331825225695" TEXT="Tomo synthesis">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1330434668646" ID="ID_708115001" MODIFIED="1331805616474" TEXT="Find a proper reconstruction method">
<node COLOR="#111111" CREATED="1331804656684" ID="ID_1184626489" MODIFIED="1331805616474" TEXT="Is parallel beam a valid approximation"/>
<node COLOR="#111111" CREATED="1331804696943" ID="ID_912276677" MODIFIED="1331805616474" TEXT="Does it facilitate the method choice if cone beam is used"/>
</node>
<node COLOR="#111111" CREATED="1330434648434" ID="ID_974402513" MODIFIED="1331805616474" TEXT="Evaluate the feasibility of TS"/>
<node COLOR="#111111" CREATED="1330435179750" ID="ID_1626478664" MODIFIED="1331805616474" TEXT="Is the information quantitative?">
<node COLOR="#111111" CREATED="1330435204018" ID="ID_611377431" MODIFIED="1331805616474" TEXT="What is needed to make the lin attn coef quantitative"/>
</node>
<node COLOR="#111111" CREATED="1330434691557" ID="ID_109583365" MODIFIED="1331805616475" TEXT="Identify boundary conditions for the acquisition">
<node COLOR="#111111" CREATED="1330435115887" ID="ID_421930653" MODIFIED="1331805616475" TEXT="Min scan arc">
<node COLOR="#111111" CREATED="1331804506979" ID="ID_1755476346" MODIFIED="1331805616475" TEXT="How remote is the sample to support this arc?"/>
<node COLOR="#111111" CREATED="1331804575271" ID="ID_1116640709" MODIFIED="1331805616475" TEXT="Does the geometric blurring have a relevant impact on the result"/>
</node>
<node COLOR="#111111" CREATED="1330435124786" ID="ID_137085254" MODIFIED="1331805616475" TEXT="Estimate root contrast"/>
<node COLOR="#111111" CREATED="1331804214533" ID="ID_1678717501" MODIFIED="1331805616475" TEXT="How many projections are needed"/>
<node COLOR="#111111" CREATED="1331804223973" ID="ID_929416679" MODIFIED="1331805616475" TEXT="Which projection contrast is needed"/>
</node>
</node>
<node COLOR="#990000" CREATED="1330433871810" ID="ID_357300847" MODIFIED="1331890670081" TEXT="Spatio temporal acquisition strategies">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1330434996976" ID="ID_1803880837" MODIFIED="1331805616476" TEXT="Evaluation of ST for root/soil applications"/>
<node COLOR="#111111" CREATED="1330434799337" ID="ID_55464563" MODIFIED="1331805616476" TEXT="Investigate the usefulness of ST for tomosynthesis">
<node COLOR="#111111" CREATED="1331804261042" ID="ID_1852379317" MODIFIED="1331805616477" TEXT="How does the reconstructor cope with ST data?"/>
</node>
<node COLOR="#111111" CREATED="1331804457003" ID="ID_860900750" MODIFIED="1331805616477" TEXT="Can the CT data be evaluated as radiograpies"/>
<node COLOR="#111111" CREATED="1331890674065" ID="ID_835197139" MODIFIED="1331890699213" TEXT="Investigate overlapping time frames of projection data"/>
</node>
<node COLOR="#990000" CREATED="1330433585091" FOLDED="true" ID="ID_1700277124" MODIFIED="1331825227471" TEXT="Verification models">
<font NAME="SansSerif" SIZE="14"/>
<node COLOR="#111111" CREATED="1330433612649" ID="ID_1290569992" MODIFIED="1331805616477" TEXT="Forward projector"/>
<node COLOR="#111111" CREATED="1331804298083" ID="ID_299295796" MODIFIED="1331805616478" TEXT="Model a numerical ground truth"/>
<node COLOR="#111111" CREATED="1330433622073" ID="ID_1415659542" MODIFIED="1331805616478" TEXT="Montecarlo modelling"/>
</node>
</node>
<node COLOR="#00b439" CREATED="1330433691774" ID="ID_1187113626" MODIFIED="1331805616478" TEXT="Scattering correction">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
</node>
<node COLOR="#00b439" CREATED="1330433936008" ID="ID_761186922" MODIFIED="1331825235241" TEXT="Limiting parameters">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
<node COLOR="#990000" CREATED="1331804126113" ID="ID_333789048" MODIFIED="1331805616479" TEXT="Which sample dimensions are needed">
<font NAME="SansSerif" SIZE="14"/>
</node>
<node COLOR="#990000" CREATED="1331804167805" ID="ID_461587187" MODIFIED="1331805616479" TEXT="How low time can be used for a scan">
<font NAME="SansSerif" SIZE="14"/>
</node>
<node COLOR="#990000" CREATED="1331804189986" ID="ID_1687956878" MODIFIED="1331805616480" TEXT="">
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#00b439" CREATED="1330434097520" ID="ID_1751313812" MODIFIED="1331825234070" TEXT="Experiments">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
<node COLOR="#990000" CREATED="1331807237932" ID="ID_1718252604" MODIFIED="1331807284482" TEXT="Design sample containers that allow the plant experiment in the given environment">
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
</node>
<node COLOR="#0033ff" CREATED="1330433410615" FOLDED="true" ID="ID_508555544" MODIFIED="1331889171504" POSITION="right" TEXT="Neutron related correction procedures">
<edge STYLE="sharp_bezier" WIDTH="8"/>
<font NAME="SansSerif" SIZE="18"/>
<node COLOR="#00b439" CREATED="1330434748658" ID="ID_467534299" MODIFIED="1331805616481" TEXT="Revisit QNI for thin slabs with high water content">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
</node>
</node>
<node COLOR="#0033ff" CREATED="1331889820901" ID="ID_259858884" MODIFIED="1331889831608" POSITION="left" TEXT="Root properties">
<edge STYLE="sharp_bezier" WIDTH="8"/>
<font NAME="SansSerif" SIZE="18"/>
</node>
<node COLOR="#0033ff" CREATED="1331825582068" HGAP="98" ID="ID_975041083" MODIFIED="1331889932762" POSITION="left" TEXT="Rhizosphere" VSHIFT="1">
<edge STYLE="sharp_bezier" WIDTH="8"/>
<font NAME="SansSerif" SIZE="18"/>
<node COLOR="#00b439" CREATED="1331825691046" ID="ID_1336926958" MODIFIED="1331825703090" TEXT="Fundamental understanding missing">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
</node>
<node COLOR="#00b439" CREATED="1331825705124" ID="ID_597778440" MODIFIED="1331825734819" TEXT="Root function">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
<node COLOR="#990000" CREATED="1331825819920" ID="ID_1498372830" MODIFIED="1331825831870" TEXT="What are they doing">
<font NAME="SansSerif" SIZE="14"/>
</node>
<node COLOR="#990000" CREATED="1331825832497" ID="ID_68383008" MODIFIED="1331825875058" TEXT="Not explicitly look at morphology and growth">
<font NAME="SansSerif" SIZE="14"/>
</node>
<node COLOR="#990000" CREATED="1331825987959" ID="ID_442624588" MODIFIED="1331826005356" TEXT="3D information is essential for the modelling">
<font NAME="SansSerif" SIZE="14"/>
</node>
<node COLOR="#990000" CREATED="1331826015551" ID="ID_1609339613" MODIFIED="1331826027765" TEXT="How does the water flow in the root">
<font NAME="SansSerif" SIZE="14"/>
</node>
</node>
<node COLOR="#00b439" CREATED="1331825735467" ID="ID_1881964352" MODIFIED="1331825779150" TEXT="Learn how to use measurement tools">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
</node>
</node>
<node COLOR="#0033ff" CREATED="1330433705796" HGAP="142" ID="ID_1979539747" MODIFIED="1331889941603" POSITION="left" TEXT="Root water uptake" VSHIFT="7">
<edge STYLE="sharp_bezier" WIDTH="8"/>
<font NAME="SansSerif" SIZE="18"/>
<node COLOR="#00b439" CREATED="1330433997544" ID="ID_1054744468" MODIFIED="1331805616483" TEXT="Generation of synthetic root structures for simulation">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
</node>
<node COLOR="#00b439" CREATED="1330434036911" ID="ID_1802323191" MODIFIED="1331805616483" TEXT="Use synthetic roots in hydraulic models">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
</node>
<node COLOR="#00b439" CREATED="1330434060760" ID="ID_767754384" MODIFIED="1331805616484" TEXT="Use synthetic roots in neutron transmission models">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
</node>
</node>
<node COLOR="#0033ff" CREATED="1330434960062" HGAP="84" ID="ID_16390905" MODIFIED="1331889886439" POSITION="left" TEXT="Model of water flow in root and rhizospere" VSHIFT="8">
<edge STYLE="sharp_bezier" WIDTH="8"/>
<font NAME="SansSerif" SIZE="18"/>
</node>
<node COLOR="#0033ff" CREATED="1331803900225" FOLDED="true" ID="ID_763852026" MODIFIED="1331889172337" POSITION="right" TEXT="Comparison to X-ray root imaging">
<edge STYLE="sharp_bezier" WIDTH="8"/>
<font NAME="SansSerif" SIZE="18"/>
<node COLOR="#00b439" CREATED="1331803931721" ID="ID_1400038287" MODIFIED="1331805616485" TEXT="Scan the same root sample with both modalities (NEUTRA)">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
</node>
<node COLOR="#00b439" CREATED="1331803952208" ID="ID_858677159" MODIFIED="1331805616485" TEXT="Compare the representation of relevant features">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
</node>
<node COLOR="#00b439" CREATED="1331803976844" ID="ID_1167111649" MODIFIED="1331805616486" TEXT="Compare processing method performance">
<edge STYLE="bezier" WIDTH="thin"/>
<font NAME="SansSerif" SIZE="16"/>
</node>
</node>
<node COLOR="#0033ff" CREATED="1331807583044" HGAP="-186" ID="ID_685464015" MODIFIED="1331889388786" POSITION="left" TEXT="Interfacing image data to physical" VSHIFT="71">
<edge STYLE="sharp_bezier" WIDTH="8"/>
<font NAME="SansSerif" SIZE="18"/>
</node>
</node>
</map>
