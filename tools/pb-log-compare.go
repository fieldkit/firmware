package main

import (
	"bufio"
	"encoding/hex"
	"fmt"
	"os"
	"regexp"
	"strings"

	"github.com/r3labs/diff"

	"github.com/golang/protobuf/proto"

	pb "github.com/fieldkit/data-protocol"
)

func main() {
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

	dataRecordsByKey := make(map[string]*pb.DataRecord)
	dataRecords := make([]*pb.DataRecord, 0)

	for key, value := range blocks {
		bytes, err := hex.DecodeString(value)
		if err != nil {
			panic(err)
		}

		var signedRecord pb.SignedRecord
		var dataRecord pb.DataRecord

		buffer := proto.NewBuffer(bytes)
		size, err := buffer.DecodeVarint()
		if err != nil {
			panic(err)
		}

		err = buffer.Unmarshal(&dataRecord)
		if err != nil {
			panic(err)
		}

		fmt.Printf("%+v\n", dataRecord)

		dataRecordsByKey[key] = &dataRecord
		dataRecords = append(dataRecords, &dataRecord)

		_ = signedRecord
		_ = dataRecord
		_ = size
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

	_ = dataRecordsByKey
}
