package main

import (
	"bufio"
	"encoding/hex"
	"flag"
	"fmt"
	"os"
	"regexp"
	"strings"

	"github.com/r3labs/diff"

	"github.com/golang/protobuf/proto"

	pb "github.com/fieldkit/data-protocol"
)

type options struct {
	Lora      bool
	Data      bool
	Signed    bool
	Delimited bool
}

func main() {
	o := options{}

	flag.BoolVar(&o.Delimited, "delimited", false, "")
	flag.BoolVar(&o.Lora, "lora", false, "")
	flag.BoolVar(&o.Data, "data", false, "")
	flag.BoolVar(&o.Signed, "signed", false, "")

	flag.Parse()

	bytesRe := regexp.MustCompile("\\(\\d+ bytes\\)")

	blocks := make(map[string]string)

	scanner := bufio.NewScanner(os.Stdin)

	for scanner.Scan() {
		line := scanner.Text()

		halves := strings.Split(bytesRe.ReplaceAllString(line, ""), " ")
		key := halves[0]
		hexEncoded := strings.Join(halves[1:], "")

		if key != "" {
			blocks[key] += hexEncoded
		}
	}

	dataRecords := make([]*pb.DataRecord, 0)
	signedRecords := make([]*pb.SignedRecord, 0)
	loraRecords := make([]*pb.LoraRecord, 0)

	for key, value := range blocks {
		bytes, err := hex.DecodeString(value)
		if err != nil {
			panic(err)
		}

		size := uint64(0)
		buffer := proto.NewBuffer(bytes)
		if o.Delimited {
			size, err = buffer.DecodeVarint()
			if err != nil {
				panic(err)
			}
		}

		if o.Data {
			var dataRecord pb.DataRecord
			err = buffer.Unmarshal(&dataRecord)
			if err == nil {
				fmt.Printf("%+v\n", dataRecord)
				dataRecords = append(dataRecords, &dataRecord)
			} else if o.Data {
				fmt.Printf("%v\n", err)
			}
		}

		if o.Signed {
			var signedRecord pb.SignedRecord
			err = buffer.Unmarshal(&signedRecord)
			if err == nil {
				fmt.Printf("%+v\n", signedRecord)
				signedRecords = append(signedRecords, &signedRecord)
			} else if o.Signed {
				fmt.Printf("%v\n", err)
			}
		}

		if o.Lora {
			var loraRecord pb.LoraRecord
			err = buffer.Unmarshal(&loraRecord)
			if err == nil {
				fmt.Printf("%+v\n", loraRecord)
				loraRecords = append(loraRecords, &loraRecord)
			} else if o.Lora {
				fmt.Printf("%v\n", err)
			}
		}

		_ = size
		_ = key
	}

	if len(dataRecords) == 2 {
		changelog, err := diff.Diff(dataRecords[0], dataRecords[1])
		if err != nil {
			panic(err)
		}

		fmt.Println()

		for _, change := range changelog {
			fmt.Printf("%+v\n", change)
		}
	}

	_ = signedRecords
	_ = loraRecords
}
