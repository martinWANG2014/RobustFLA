#!/usr/bin/env bash
if [[ $# -eq 1 ]] ; then
    outfile=$(echo $1 | sed -e 's/txt/json/g')
    case $(basename $1) in
        "flight.txt")
            echo "[INFO] convert flight.txt to flight.json"
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
                    for(i=0;i<int(n/2);i++){
                        if(pointlist[i*2+2]>pointlist[(i+1)*2+2]){
                            break;
                        }
                        print "\t\t\t\42" i"\42: {";
                        gsub (" ", "", pointlist[i*2+1]);
                        print "\t\t\t\t\42Code\42: \42" pointlist[i*2+1] "\42,";
                        print "\t\t\t\t\42Time\42: " pointlist[i*2+2];
                        print "\t\t\t},";
                    }
                    print "\t\t\t\42" i"\42: {";
                    gsub (" ", "", pointlist[i*2+1]);
                    print "\t\t\t\t\42Code\42: \42" pointlist[i*2+1] "\42,";
                    print "\t\t\t\t\42Time\42: " pointlist[i*2+2];
                    print "\t\t\t},";
                    print "\t\t\t\42PLN\42: " i+1 "";
                    print "\t\t}";
                    print "\t},";
                }
                END{print "}" }' $1 | sed 'N;$s/,\n/\n/;P;D' > ${outfile}
            ;;
        "beacon.txt")
            echo "[INFO] convert beacon.txt to beacon.json"
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
            echo "[INFO] convert airport.txt to airport.json"
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
    echo "[INFO] target file: $outfile has been created successfully!"
else
    echo "Usage: convertTXT2JSON.sh filename"
fi


