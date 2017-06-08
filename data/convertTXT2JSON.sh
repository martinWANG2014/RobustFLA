#!/usr/bin/env bash

if [[ $# -eq 1 ]] ; then
    outfile=$(echo $1 | sed -e 's/txt/json/g')
    echo "target file: $outfile"
    case $(basename $1) in
#        "config.txt")
#            echo "convert config file"
#            awk 'BEGIN{ print "{" }
#                NR == 2 {
#                    print "\t\42Periods\42: " $1 ","
#                }
#                NR == 3 {
#                    gsub ("txt", "json", $1);
#                    print "\t\42Airport\42: \42" $1 "\42,"
#                }
#                NR == 4 {
#                    gsub ("txt", "json", $1);
#                    print "\t\42Beacon\42: \42" $1 "\42,"
#                }
#                NR == 5 {
#                    gsub ("txt", "json", $1);
#                    print "\t\42Flight\42: \42" $1 "\42,"
#                }
#                END{print "}" }' $1 | sed 'N;$s/,\n/\n/;P;D' > ${outfile}
#            ;;
        "flight.txt")
            echo "convert flight file"
            awk -F ';' 'BEGIN{ print "{" }
                NR == 2 {
                    print "\t\42FN\42: " $1","
                }
                NR > 2 {
                    print "\t\42" NR-3 "\42: {";
                    print "\t\t\42Origin\42: \42" $1 "\42,";
                    print "\t\t\42Dest\42: \42" $2 "\42,";
                    print "\t\t\42FCode\42: \42" $3 "\42,";
                    print "\t\t\42DTime\42: " $4 ",";
                    print "\t\t\42FLevel\42: " $5 ",";
                    print "\t\t\42PointList\42: {";
                    n=split($6, pointlist, ":");
                    print "\t\t\t\42PLN\42: " int(n/2) ",";
                    for(i=0;i<int(n/2)-1;i++){
                        print "\t\t\t\42" i"\42: {";
                        gsub (" ", "", pointlist[i*2+1]);
                        print "\t\t\t\t\42Code\42: \42" pointlist[i*2+1] "\42,";
                        print "\t\t\t\t\42Time\42: " pointlist[i*2+2];
                        print "\t\t\t},";
                    }
                    print "\t\t\t\42" int(n/2)-1 "\42: {";
                    gsub (" ", "", pointlist[n-2]);
                    print "\t\t\t\t\42Code\42: \42" pointlist[n-2] "\42,";
                    print "\t\t\t\t\42Time\42: " pointlist[n-1];
                    print "\t\t\t}";
                    print "\t\t}";
                    print "\t},"
                }
                END{print "}" }' $1 | sed 'N;$s/,\n/\n/;P;D' > ${outfile}
            ;;
        "beacon.txt")
            echo "convert beacon file"
            awk 'BEGIN{ print "{" }
                NR == 2 {
                    split($0, a, ";");
                    print "\t\42BN\42: " a[1]","
                }
                NR > 2 {
                    print "\t\42" NR-3 "\42: {";
                    print "\t\t\42Code\42: \42" $1 "\42,";
                    print "\t\t\42Lat\42: " $2 ",";
                    print "\t\t\42Lng\42: " $3 ",";
                    printf "\t\t\42Name\42: \42";
                    for(i=4;i<NF;i++){
                        printf "%s ", $i
                    }
                    gsub(/\r/,"",$NF);
                    gsub(/\n/,"",$NF);
                    print $NF"\42";
                    print "\t},"
                }
                END{print "}" }' $1 | sed 'N;$s/,\n/\n/;P;D' > ${outfile}
            ;;
        "airport.txt")
            echo "convert airport file"
            awk 'BEGIN{ print "{" }
                NR == 2 {
                    split($0, a, ";");
                    print "\t\42AN\42: " a[1]","
                }
                NR > 2{
                    print "\t\42" NR-3 "\42: {";
                    print "\t\t\42ICAO\42: \42" $1 "\42,";
                    print "\t\t\42Lat\42: " $2 ",";
                    print "\t\t\42Lng\42: " $3 ",";
                    printf "\t\t\42Name\42: \42";
                    for(i=4;i<NF;i++){
                        printf "%s ", $i
                    }
                    gsub(/\r/,"",$NF);
                    gsub(/\n/,"",$NF);
                    print $NF"\42";
                    print "\t},"
                }
                END{print "}"}' $1  | sed 'N;$s/,\n/\n/;P;D' > ${outfile}
            ;;
        *)
            echo "not support such file"
            exit 1
    esac
else
    echo "Usage: convertTXT2JSON.sh filename"
fi


