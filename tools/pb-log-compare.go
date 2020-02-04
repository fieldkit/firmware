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

	"github.com/k0kubun/pp"

	"github.com/golang/protobuf/proto"

	pbapp "github.com/fieldkit/app-protocol"
	pbdata "github.com/fieldkit/data-protocol"
)

type options struct {
	Delimited bool
	Lora      bool
	Data      bool
	Signed    bool
	HttpReply bool
}

func main() {
	o := options{}

	flag.BoolVar(&o.Delimited, "delimited", false, "")
	flag.BoolVar(&o.Lora, "lora", false, "")
	flag.BoolVar(&o.Data, "data", false, "")
	flag.BoolVar(&o.Signed, "signed", false, "")
	flag.BoolVar(&o.HttpReply, "http-reply", false, "")

	flag.Parse()

	bytesRe := regexp.MustCompile("\\(\\d+ bytes\\)")

	blocks := make(map[string]string)

	scanner := bufio.NewScanner(os.Stdin)

	fmt.Printf("%+v\n", o)

	for scanner.Scan() {
		line := scanner.Text()

		halves := strings.Split(bytesRe.ReplaceAllString(line, ""), " ")
		key := halves[0]
		hexEncoded := strings.Join(halves[1:], "")

		if key != "" {
			blocks[key] += hexEncoded
		}
	}

	fmt.Printf("blocks: %d\n", len(blocks))

	dataRecords := make([]*pbdata.DataRecord, 0)
	signedRecords := make([]*pbdata.SignedRecord, 0)
	loraRecords := make([]*pbdata.LoraRecord, 0)
	httpReplies := make([]*pbapp.HttpReply, 0)

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
			var dataRecord pbdata.DataRecord
			err = buffer.Unmarshal(&dataRecord)
			if err == nil {
				pp.Print(dataRecord)
				fmt.Println()
				dataRecords = append(dataRecords, &dataRecord)
			} else {
				fmt.Printf("error parsing data: %v\n", err)
			}
		}

		if o.Signed {
			var signedRecord pbdata.SignedRecord
			err = buffer.Unmarshal(&signedRecord)
			if err == nil {
				pp.Print(signedRecord)
				fmt.Println()
				signedRecords = append(signedRecords, &signedRecord)
			} else {
				fmt.Printf("%error parsing signed: v\n", err)
			}
		}

		if o.Lora {
			var loraRecord pbdata.LoraRecord
			err = buffer.Unmarshal(&loraRecord)
			if err == nil {
				pp.Print(loraRecord)
				fmt.Println()
				loraRecords = append(loraRecords, &loraRecord)
			} else {
				fmt.Printf("error parsing lora: %v\n", err)
			}
		}

		if o.HttpReply {
			var httpReply pbapp.HttpReply
			err = buffer.Unmarshal(&httpReply)
			if err == nil {
				pp.Print(httpReply)
				fmt.Println()
				httpReplies = append(httpReplies, &httpReply)
			} else {
				fmt.Printf("error parsing app: %v\n", err)
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

	if len(httpReplies) == 2 {
		changelog, err := diff.Diff(httpReplies[0], httpReplies[1])
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
