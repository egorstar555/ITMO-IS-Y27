package com.Starygin.service;

import com.Starygin.service.dto.OwnerDto;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.kafka.requestreply.ReplyingKafkaTemplate;
import org.springframework.kafka.requestreply.RequestReplyFuture;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

@Service
public class OwnerServiceKafka {

    private final ObjectMapper mapper;

    private final ReplyingKafkaTemplate<String, Object, Object> replyingKafkaTemplate;

    @Autowired
    public OwnerServiceKafka(ReplyingKafkaTemplate<String, Object, Object> replyingOwnerTemplate, ObjectMapper objectMapper) {
        this.replyingKafkaTemplate = replyingOwnerTemplate;
        this.mapper = objectMapper;
    }

    public List<OwnerDto> findAll() throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        return mapper.readValue(sendRequestAndReceiveResponse("owners.findAll", ""), new TypeReference<List<OwnerDto>>() {
        });
    }

    public OwnerDto findOne(Long id) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        String request = mapper.writeValueAsString(id);

        return mapper.readValue(sendRequestAndReceiveResponse("owners.findOne", request), OwnerDto.class);
    }

    public List<OwnerDto> findByName(String name) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        return mapper.readValue(sendRequestAndReceiveResponse("owners.findByName", name), new TypeReference<List<OwnerDto>>() {
        });
    }

    public OwnerDto create(OwnerDto ownerDto) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        String request = mapper.writeValueAsString(ownerDto);

        return mapper.readValue(sendRequestAndReceiveResponse("owners.create-owner", request), OwnerDto.class);
    }

    public void delete(Long id) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        String request = mapper.writeValueAsString(id);

        sendRequestAndReceiveResponse("owners.delete-owner", request);
    }

    public OwnerDto updateOwner(OwnerDto ownerDto) throws ExecutionException, InterruptedException, TimeoutException, JsonProcessingException {
        String request = mapper.writeValueAsString(ownerDto);

        return mapper.readValue(sendRequestAndReceiveResponse("owners.update-owner", request), OwnerDto.class);
    }

    private String sendRequestAndReceiveResponse(String topic, Object payload) throws ExecutionException, InterruptedException, TimeoutException {
        ProducerRecord<String, Object> record = new ProducerRecord<>(topic, payload);
        RequestReplyFuture<String, Object, Object> replyFuture = replyingKafkaTemplate.sendAndReceive(record);
        ConsumerRecord<String, Object> response = replyFuture.get(10, TimeUnit.SECONDS);

        return response.value().toString();
    }
}